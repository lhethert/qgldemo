#include <list>

#include <QGLFunctions>
#include <QGLBuffer>
#include <QPaintDevice>
#include <QSharedPointer>
#include <QColor>
#include <QMap>

#include "Scene/transformation.h"
#include "Scene/camera.h"
#include "Scene/scene.h"
#include "Scene/mesh.h"
#include "Scene/meshinstance.h"
#include "Scene/helpers.h"
#include "Math/matrix4.h"
#include "glrenderer.h"
#include "glutils.h"
#include "shader.h"

namespace GLDemo
{
    namespace
    {
        /**
         * \internal Class representing GL buffer data for a particular element type.
         *           Used to cache the buffer data associated with a particular mesh.
         */
        class IndexBufferData
        {
        public:
            IndexBufferData(ElementList::ElementType type, const QGLBuffer& buffer, int numIndices) :
                m_type(type),
                m_indexData(buffer),
                m_numIndices(numIndices)
            {
            }

            // Make these public so they can be cheaply accessed from the cache item.
            ElementList::ElementType m_type;
            QGLBuffer                m_indexData;
            int                      m_numIndices;
        };

        typedef std::list<IndexBufferData> IndexDataList;

        /**
         * \internal Class for caching GL mesh data once it's been created.
         */
        class CachedMesh
        {
        public:
            CachedMesh(const QString& meshId) : m_meshId(meshId) {}

            // Make these public so they can be cheaply accessed from the cache item.
            QString       m_meshId;
            QGLBuffer     m_vertexData;
            IndexDataList m_indexData;
        };
    }

    /**
     * \internal
     * We extend QGLFunctions so we get access to OpenGL functions here. We extend
     * it publicly so that shaders can access the class without having to create their
     * own instance of the QGLFunctions class.
     */
    class GLRendererImpl : public QGLFunctions
    {
    public:
        typedef QMap<QString, CachedMesh> MeshDataCache;

        GLRendererImpl(const GLRendererImpl&);
        GLRendererImpl& operator=(const GLRendererImpl&);

        GLRenderer&    m_renderer;
        QPaintDevice&  m_paintDevice;
        MeshDataCache  m_meshCache;
        int            m_width;
        int            m_height;
        bool           m_initialized;
        Matrix4f       m_matProj;
        Matrix4f       m_matView;
        Matrix4f       m_matViewInv;
        Camera*        m_camera;

        GLRendererImpl(GLRenderer& renderer, QPaintDevice& device);
        ~GLRendererImpl();

        void  setCamera(Camera* camera);

        bool  process(const MeshInstance& instance);

        void  setupViewport(int x, int y, int width, int height);
        bool  setupMatrices(Scene& scene);

        bool  preRender(Scene& scene);
        bool  renderScene(Scene& scene);
        bool  postRender(Scene& scene);

        bool  initialize();
        bool  resize(int width, int height);
    };


    /**
     *
     */
    GLRendererImpl::GLRendererImpl(GLRenderer& renderer, QPaintDevice& device) :
        m_renderer(renderer),
        m_paintDevice(device),
        m_width(device.width()),
        m_height(device.height()),
        m_initialized(false),
        m_camera(0)
    {
    }


    /**
     *
     */
    GLRendererImpl::~GLRendererImpl()
    {
        // We don't need to worry about cleaning up our allocated QGLBuffers,
        // as the destructor of the QGLBuffer object does this for us, according
        // to the Qt documentation.
    }


    /**
     *
     */
    bool GLRendererImpl::setupMatrices(Scene& scene)
    {
        // Set up our perspective matrix
        const float aspectRatio = static_cast<float>(m_width) / m_height;
        const float fov         = m_camera->getFieldOfView();
        const float zNear       = m_camera->getNearPlaneDistance();
        const float zFar        = m_camera->getFarPlaneDistance();

        m_matProj.makePerspectiveProjectionFOV(fov, aspectRatio, zNear, zFar);
        m_camera->toViewMatrix(m_matView);
        m_matViewInv = m_matView.inverse();
        return true;
    }


    /**
     * \param width  Width of the viewport.
     * \param height Height of the viewport.
     */
    void GLRendererImpl::setupViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }


    /**
     *
     */
    void GLRendererImpl::setCamera(Camera* camera)
    {
        m_camera = camera;
    }


    /**
     * \param instance  The mesh instance to process
     *
     * Processes a mesh instance, generating the appropriate OpenGL data structures
     * if the mesh hasn't before been rendered.
     */
    bool  GLRendererImpl::process(const MeshInstance& instance)
    {
        PtrMesh ptrMesh = instance.getMesh();
        if (ptrMesh.isNull())
        {
            std::cout << "ERROR: Mesh " << instance.instanceName() << " contents are invalid." << std::endl;
            return false;
        }
        Mesh& mesh = *ptrMesh;

        // For both our vertices and all our primitives, we check to see whether a buffer exists
        // for our data, and if not, we create one.
        MeshDataCache::iterator meshIter = m_meshCache.find(mesh.instanceName());
        if (m_meshCache.end() == meshIter)
        {
            CachedMesh cachedMesh(mesh.instanceName());

            // Qt does shallow copy, so we can copy buffers around in "shallow" manner.
            // Start by creating our vertex data.
            {
                QGLBuffer vbo(QGLBuffer::VertexBuffer);
                vbo.setUsagePattern(QGLBuffer::StreamDraw);
                if ( !vbo.create() )
                {
                    std::cout << "ERROR: Failed to create vertex buffer object." << std::endl;
                    return false;
                }

                vbo.bind();
                std::vector<Vertex>& vertices = mesh.getVertices();
                vbo.allocate(&vertices.front(), vertices.size() * sizeof(Vertex));
                cachedMesh.m_vertexData = vbo;
            }

            // Now process the elements.
            std::list<ElementList>& elementLists = mesh.getElementLists();
            for (std::list<ElementList>::iterator elIter = elementLists.begin(); elIter != elementLists.end(); ++elIter)
            {
                QGLBuffer vbo(QGLBuffer::IndexBuffer);
                vbo.setUsagePattern(QGLBuffer::StreamDraw);
                if ( !vbo.create() )
                {
                    std::cout << "ERROR: Failed to create index buffer object." << std::endl;
                    return false;
                }

                vbo.bind();
                std::vector<unsigned>& indices = elIter->getIndices();
                vbo.allocate(&indices.front(), indices.size() * sizeof(unsigned));
                cachedMesh.m_indexData.push_front( IndexBufferData(elIter->getElementType(), vbo, indices.size()) );
            }

            meshIter = m_meshCache.insert(mesh.instanceName(), cachedMesh);
        }

        // Now that we've created the data, we can bind and render it.
        CachedMesh& glMesh = *meshIter;

        // Activate the shader
        const PtrShader& ptrShader = instance.getShader();
        if (ptrShader.isNull())
        {
            std::cout << "ERROR: Mesh instance must have a valid shader in order to be rendered." << std::endl;
            return false;
        }

        // Compute our matrices, starting by obtaining the world matrix.
        Matrix4f matWorld;
        instance.getWorldTransformation().toMatrix(matWorld);
        Matrix4f matWorldView(m_matView * matWorld);
        Matrix4f matWorldViewInvTranspose(matWorldView.inverse().transpose());
        Matrix4f matWorldViewProj(m_matProj * matWorldView);
        if (!ptrShader->activate(m_matView, matWorldView, matWorldViewInvTranspose, matWorldViewProj))
        {
            std::cout << "ERROR: Failed to activate shader." << std::endl;
            return false;
        }

        // Bind our vertex data to the appropriate attribute locations.
        int stride = 8 * sizeof(GLfloat);
        glMesh.m_vertexData.bind();
        glVertexAttribPointer(GLRenderer::Position, 3, GL_FLOAT, false, stride, 0);
        glVertexAttribPointer(GLRenderer::Normal, 3, GL_FLOAT, false, stride, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(GLRenderer::Position);
        glEnableVertexAttribArray(GLRenderer::Normal);

        // Now render each set of elements.
        IndexBufferData* indices = 0;
        for (std::list<IndexBufferData>::iterator iIter = glMesh.m_indexData.begin(); iIter != glMesh.m_indexData.end(); ++iIter)
        {
            indices = &*iIter;
            indices->m_indexData.bind();
            switch (indices->m_type)
            {
            case ElementList::TRI_LIST:
                glDrawElements(indices->m_type, 3 * indices->m_numIndices, GL_UNSIGNED_INT, 0);
                break;
            default:
                std::cout << "ERROR: Unable to render element type " << indices->m_type << std::endl;
            }
        }

        return GL_GOOD_STATE();
    }


    /**
     *
     */
    bool  GLRendererImpl::preRender(Scene& scene)
    {
        // Make sure the relevant initialization has taken place prior to performing any rendering.
        assert(m_initialized);

        // Ensure we have a camera set
        if (!m_camera)
        {
            return false;
        }

        // Need to setup the view and projection matrices in preparation for rendering.
        if (!setupMatrices(scene))
            return false;

#ifndef NDEBUG
#ifdef Q_OS_OSX
        // On newer versions of OSX, it's possible (somehow) to create a widget before creating
        // a main framebuffer. This causes OpenGL to return the GL_FRAMEBUFFER_UNDEFINED
        // error, and god knows what problems this causes if we continue to render to a context
        // with no current draw framebuffer.
        GLenum framebufferStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
        assert(framebufferStatus != GL_FRAMEBUFFER_UNDEFINED);
#endif
#endif

        // Clear the buffers in preparation for redrawing the scene.
        const QColor& bc = QColor(255, 255, 255, 255);
        glClearColor( bc.redF(), bc.greenF(), bc.blueF(), bc.alphaF() );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        return GL_GOOD_STATE();
    }


    /**
     *
     */
    bool  GLRendererImpl::renderScene(Scene &scene)
    {
        // Currently, we are rendering directly as we process each item in the scene. This is less than ideal,
        // as what we'd really like to do is sort each item so that we can efficiently render them. The best way
        // to do this is to generate a queue of items, sorted according to some pre-generated key.
        if (!scene.getRootNode().draw(&m_renderer))
        {
            std::cout << "ERROR: Failed to draw scene." << std::endl;
            return false;
        }

        return GL_GOOD_STATE();
    }


    /**
     *
     */
    bool  GLRendererImpl::postRender(Scene& scene)
    {
        return GL_GOOD_STATE();
    }


    /**
     * Initializes the OpenGL context - must be called only after the context has been created.
     * Cannot be invoked from the constructor.
     *
     * The core activity involved in the initialisation are ensuring the required OpenGL version
     * and OpenGL extensions are supported by and ready to use on the current platform.
     */
    bool  GLRendererImpl::initialize()
    {
        if (m_initialized)
            return true;

        initializeGLFunctions();

        // Set up our 'permanently' enabled GL states.
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        m_initialized = true;
        return GL_GOOD_STATE();
    }


    /**
     *
     */
    bool  GLRendererImpl::resize(int width, int height)
    {
        m_width  = width;
        m_height = height;

        setupViewport(0, 0, width, height);

        return true;
    }


    //=============================//


    /**
     * Creates a new GLRenderer, used for drawing a Scene using OpenGL.
     *
     * \note While creating a GLRenderer does not require a current OpenGL context,
     *       deleting one does, as any dedicated resources used by the renderer will potentially be
     *       cleaned up at the time it is deleted.
     */
    GLRenderer::GLRenderer(QPaintDevice& device) :
        m_pImpl(static_cast<GLRendererImpl*>(0))
    {
        m_pImpl = new GLRendererImpl(*this, device);
    }


    /**
     *
     */
    GLRenderer::~GLRenderer()
    {
        delete m_pImpl;
    }


    /**
     * \param camera The camera the renderer should use.
     */
    void  GLRenderer::setCamera(Camera* camera)
    {
        m_pImpl->setCamera(camera);
    }


    /**
     * \return The current camera.
     */
    Camera* GLRenderer::getCamera()
    {
        return m_pImpl->m_camera;
    }


    /**
     * \return The current camera.
     */
    const Camera* GLRenderer::getCamera() const
    {
        return m_pImpl->m_camera;
    }


    /**
     * \pre The context must have been made current prior to invoking this function.
     * \return True if the initialization of the renderer was successful.
     *
     * This function should be called once only after the creation of the renderer, as it requires that
     * a GL context exists. It will generally be invoked by the initialize event triggered by the GLWidget
     * parent of the renderer. If the parent of the renderer does not have such an
     * initialization event, this function will need to be invoked manually.
     *
     * No renderering will occur if initialize has not yet been called on the
     * renderer.
     */
    bool  GLRenderer::initialize()
    {
        return m_pImpl->initialize();
    }


    /**
     * \pre The context must have been made current prior to invoking this function.
     */
    bool   GLRenderer::renderScene(Scene& scene)
    {
        if (!m_pImpl->preRender(scene))
            return false;

        // Make sure we always post-render if we get past a pre-render
        bool success = true;
        if (!m_pImpl->renderScene(scene))
            success = false;

        if (!m_pImpl->postRender(scene))
            return false;

        return success;
    }


    /**
     * \pre The context must have been made current prior to invoking this function.
     * \param width  The desired width of the renderer.
     * \param height The desired height of the renderer.
     *
     * \return true if the resize was successful. The resize may fail for a number of reasons,
     * but usually only if an invalid size is specified, or it is unable to set up the required
     * off-screen buffers.
     */
    bool  GLRenderer::resize(int width, int height)
    {
        return m_pImpl->resize(width, height);
    }


    /**
     *
     */
    int GLRenderer::getWidth() const
    {
        return m_pImpl->m_width;
    }


    /**
     *
     */
    int GLRenderer::getHeight() const
    {
        return m_pImpl->m_height;
    }


    /**
     *
     */
    bool  GLRenderer::process(const MeshInstance& instance)
    {
        return m_pImpl->process(instance);
    }

}


