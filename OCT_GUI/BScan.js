var canvas3d;
var gl;

function updateTexture(texture){

}

function onInitilize(canvas) {
    canvas3d = canvas;

    // Get the context object that represents the 3D API
    gl = canvas.getContext("experimental-webgl", {depth:true});

    // Setup the OpenGL state
    gl.enable(gl.DEPTH_TEST);
    gl.depthMask(true);
    gl.enable(gl.CULL_FACE);
    gl.cullFace(gl.BACK);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    // Set viewport
    gl.viewport(0, 0, canvas.width * canvas.devicePixelRatio, canvas.height * canvas.devicePixelRatio);

}

function paintGL(canvas){
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.clearColor(1.0, 0.0, 0.0, 1.0);
}

function resizeGL(canvas){
    var pixelRatio = canvas.devicePixelRatio;
    canvas.pixelSize = Qt.size(canvas.width * pixelRatio,
                               canvas.height * pixelRatio);
    if (gl) {
        gl.viewport(0, 0,
                    canvas.width * canvas.devicePixelRatio,
                    canvas.height * canvas.devicePixelRatio);
    }
}
