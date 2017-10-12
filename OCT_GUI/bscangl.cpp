#include "bscangl.h"

static Vertex sg_vertexes[] = {
  Vertex( QVector3D( 0.00f,  0.75f, 1.0f), QVector3D(0.00f,  0.75f, 1.0f) ),
  Vertex( QVector3D( 0.75f, -0.75f, 1.0f), QVector3D(0.75f, -0.75f, 1.0f) ),
  Vertex( QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(-0.75f, -0.75f, 1.0f) )
};

struct VertexData
{
    QVector2D position;
    QVector2D texCoord;
};

BScanRenderer::BScanRenderer()
{
    //Init the texture

    //initializeOpenGLFunctions();

    glGenTextures(1, &m_texture);

    _initGeometery();

}

void BScanRenderer::_initGeometery(){

    // Compile vertex shader
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/bscan_vshader.glsl");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/bscan_fshader.glsl");
    m_program.link();
    m_program.bind();

    m_vertBuff.create();
    m_vertBuff.bind();
    m_vertBuff.allocate(sg_vertexes, sizeof(sg_vertexes));

    m_object.create();
    m_object.bind();

    int vertexLocation = m_program.attributeLocation("position");
    m_program.enableAttributeArray(vertexLocation);
    m_program.setAttributeBuffer(vertexLocation, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());

    int texcoordLocation = m_program.attributeLocation("color");
    m_program.enableAttributeArray(texcoordLocation);
    m_program.setAttributeBuffer(texcoordLocation, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    m_program.setUniformValue("texBScan", m_texture);

    m_object.release();
    m_vertBuff.release();
    m_program.release();
}

QOpenGLFramebufferObject* BScanRenderer::createFramebufferObject(const QSize &size)
{
   QOpenGLFramebufferObjectFormat format;
   format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
   // optionally enable multisampling by doing format.setSamples(4);
   m_size = size;
   return new QOpenGLFramebufferObject(size, format);
}

void BScanRenderer::render()
{
    if(m_frameData.get()){
        if(m_RunOnce){
            m_gl_logger.initialize();
            m_RunOnce = false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1024, 0, 1024, -1, 1);
        glViewport(0, 0, 1024, 1024);

        m_program.bind();
        m_object.bind();

//        glBindTexture(GL_TEXTURE_2D, m_texture);
//        error = glGetError();
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_dimensions.at(0), m_dimensions.at(1), 0,
//                     GL_LUMINANCE, GL_UNSIGNED_BYTE, m_frameData.get());
//        error = glGetError();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        error = glGetError();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        error = glGetError();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        error = glGetError();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        error = glGetError();

        // Draw cube geometry using indices from VBO 1

        glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));

        m_object.release();
        m_program.release();

        m_win->resetOpenGLState();
    }
}

void BScanRenderer::synchronize(QQuickFramebufferObject *o){
    m_dimensions = ((BScanGL*)o)->getDimensions();
    m_frameData = ((BScanGL*)o)->getFrameData();
    m_win = o->window();
}

void BScanGL::renderBScan(shared_ptr<unsigned char> data, vector<unsigned long> dim){
    m_frameData = data;
    m_dimensions = dim;
    update();
}

void BScanGL::messageLogged(const QOpenGLDebugMessage &m){
    std::cout << m.message().toStdString();
}

