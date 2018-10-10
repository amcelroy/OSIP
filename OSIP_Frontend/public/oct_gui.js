


        var g_serverIP = location.hostname; 
        var socket = new WebSocket('ws://' + g_serverIP + ':8081');
        var bscanCanvas = document.getElementById("BScanCanvas");
        var lastBuffer = {};
        var lastMessage = '';
        var sliderMinMax = document.getElementById('sliderMinMax');
        var sliderBScanSelect = document.getElementById('sliderBScanSelect');
        var socketOpen = 0;
        var g_LastEnFace = 0;
        var g_numberOfFrames = 0;
        var g_stopPlayback = 0;
        var g_currentFrame = 0;
        var g_intervalPlaybackHandle = 0;
        var g_ServerMode = "daq";
        var g_AScanPerBScan = 0;
        var g_PointsPerAScan = 0;
        var g_StartEnFace = 0;
        var g_EndEnFace = 1;
        var g_RawAScanLength = 0;
        var g_IntAScanLength= 0;
        var points_per_ascan = 0;
        var g_SelectedAScan = 0;
        var g_LastProcessedFrame = 0;
        var m_LUT = [66049,66049,66305,66561,66817,67073,67329,67585,67841,68097,68353,68609,68865,69121,69377,69633,69889,70145,70401,70657,70913,71169,71425,71681,71937,72193,72449,72705,72961,73217,73473,73729,73985,74241,74497,74753,75009,75265,75521,75777,76033,76289,76545,76801,77057,77313,77569,77825,78081,78337,78593,78849,79105,79361,79617,79873,80129,80385,80641,80897,81153,81409,81665,81921,147458,278276,409094,539912,670730,801548,932366,1063184,1194002,1324820,1455638,1586456,1717274,1848092,1978910,2109728,2240546,2371364,2502182,2633000,2763818,2894636,3025454,3156272,3287090,3417908,3548726,3679544,3810362,3941180,4071998,4202816,4268097,4398915,4529733,4660551,4791369,4922187,5053005,5183823,5314641,5445459,5576277,5707095,5837913,5968731,6099549,6230367,6361185,6492003,6622821,6753639,6884457,7015275,7146093,7276911,7407729,7538547,7669365,7800183,7931001,8061819,8192637,8323455,8454525,8586107,8717689,8849271,8980853,9112435,9244017,9375599,9507181,9638763,9770345,9901927,10033509,10165091,10296673,10428255,10559837,10691419,10823001,10954583,11086165,11217747,11349329,11480911,11612493,11744075,11875657,12007239,12138821,12270403,12401985,12533568,12599358,12730940,12862522,12994104,13125686,13257268,13388850,13520432,13652014,13783596,13915178,14046760,14178342,14309924,14441506,14573088,14704670,14836252,14967834,15099416,15230998,15362580,15494162,15625744,15757326,15888908,16020490,16152072,16283654,16415236,16546818,16678401,16678916,16679432,16679948,16680464,16680980,16681496,16682012,16682528,16683044,16683560,16684076,16684592,16685108,16685624,16686140,16686656,16687171,16687687,16688203,16688719,16689235,16689751,16690267,16690783,16691299,16691815,16692331,16692847,16693363,16693879,16694395,16694911,16695171,16695687,16696203,16696719,16697235,16697751,16698267,16698783,16699299,16699815,16700331,16700847,16701363,16701879,16702395,16702911,16703426,16703942,16704458,16704974,16705490,16706006,16706522,16707038,16707554,16708070,16708586,16709102,16709618,16710134,16710650,16710650];
        var handle_FrameRequester;
        var g_ProcessingFrame;

        window.onload = () => {
            for(var i = 0; i < m_LUT.length; i++){
                m_LUT[i] += 0xFF000000;
            }  

            var box = document.getElementById("canvasIntensityAScan");
            box.style.display = "none";            
            box = document.getElementById("canvasRawAScan");
            box.style.display = "none";
        }

        noUiSlider.create(sliderMinMax, {
            start: [-20, 40],
            connect: true,
            range: {
                'min': -50,
                'max': 50
            },
            orientation: 'vertical'
        });

        noUiSlider.create(sliderBScanSelect, {
            start: [1],
            connect: true,
            step: 1,
            range: {
                'min': 0,
                'max': 100
            },
        });

        sliderBScanSelect.noUiSlider.on('update', function(v, h){
            if(socketOpen){
                g_currentFrame = parseFloat(v[h]);
                var _json = { "request" : "frame", "frame" : parseFloat(v[h]) };
                socket.send(JSON.stringify(_json));
            }
        });

        sliderMinMax.noUiSlider.on('update', function(v, h){
            if(socketOpen){
                var _json = { "request" : "scaling", 
                                "min" : parseFloat(v[0]),
                                "max" : parseFloat(v[1]) };
                socket.send(JSON.stringify(_json));

                if(g_ServerMode === "daq"){

                }else{
                    _json = { "request" : "frame", "frame" : g_currentFrame };
                    socket.send(JSON.stringify(_json));
                }
            }
        });

        var timerHandle;
        bscanCanvas.addEventListener('mousemove', function(event){
            if(timerHandle){
                clearInterval(timerHandle);
            }
            timerHandle = setTimeout(function(){
                let cssCanvas = window.getComputedStyle(bscanCanvas);
                let w = parseInt(cssCanvas.getPropertyValue("width"), 10);
                var ascan = g_AScanPerBScan - g_AScanPerBScan*(event.offsetX / w);
                _json = { "request" : "update_ascan", "ascan" :  ascan };
                socket.send(JSON.stringify(_json));
                g_SelectedAScan = ascan;
                requestFramePlayback(0);
                clearInterval(timerHandle);
            }, 250); 
        }, false);

        bscanCanvas.addEventListener('mouseleave', function(event){
            if(timerHandle){
                clearInterval(timerHandle);
            }
        });

        bscanCanvas.addEventListener('mousedown', function(event){
            g_StartEnFace = event.offsetY;
        });

        bscanCanvas.addEventListener('mouseup', function(event){
            var tmp_end = event.offsetY;
            var max = Math.max(g_StartEnFace, tmp_end);
            var min = Math.min(g_StartEnFace, tmp_end);
            g_StartEnFace = min;
            g_EndEnFace = max;

            if(g_StartEnFace == g_EndEnFace){
                g_StartEnFace = 1;
                g_EndEnFace = g_PointsPerAScan;
            }

            var j = { "request" : "enface", "start" : g_StartEnFace, "stop" : g_EndEnFace };
            socket.send(JSON.stringify(j));
            requestFramePlayback(0);
        });

        function checkboxIntensityAScanOnChange(){
            var box = document.getElementById("canvasIntensityAScan");
            if(box.style.display === "none"){
                box.style.display = "flex";
            }else{
                box.style.display = "none";
            }
        }

        function checkboxRawAScanDisplayOnClick(){
            var box = document.getElementById("canvasRawAScan");
            if(box.style.display === "none"){
                box.style.display = "flex";
            }else{
                box.style.display = "none";
            }
        }

        function stopButtonClick(){
            var j = { "request" : "stop" };
            socket.send(JSON.stringify(j));
            clearInterval(handle_FrameRequester);
        }

        function enfaceButtonCick(){
            g_stopPlayback = 1;
            var j = { "request" : "load" };
            socket.send(JSON.stringify(j));
            clearInterval(g_intervalPlaybackHandle);
        }

        function requestFramePlayback(increment){
            if(g_currentFrame >= 0 && g_currentFrame < g_numberOfFrames){
                var _json = { "request" : "frame", "frame" : g_currentFrame };
                g_frameReady = 0;
                socket.send(JSON.stringify(_json));
                g_currentFrame += increment;
                sliderBScanSelect.noUiSlider.set(g_currentFrame);
            }else{
                //clearInterval(g_intervalPlaybackHandle);
            }
        }

        function buttonScanSubmit(){
            var json = { "request" : "set_daq" ,  
                        "points_per_ascan" : parseFloat(document.getElementById("textPointsPerAScan").getAttribute("value")) ,
                        "a_per_b" : parseFloat(document.getElementById("textPointsPerAScan").getAttribute("value")) ,
                        "bscans" : parseFloat(document.getElementById("textBScans").getAttribute("value")) ,
                        "start_trim" : parseFloat(document.getElementById("textStartTrim").getAttribute("value")) ,
                        "stop_trim" : parseFloat(document.getElementById("textStopTrim").getAttribute("value")) ,
                        "voltage" : parseFloat(document.getElementById("textVoltage").getAttribute("value")) ,
                        "bits" : parseFloat(document.getElementById("textBits").getAttribute("value")) ,
                        "fast_axis_amp" : parseFloat(document.getElementById("textFastAxisAmplitude").getAttribute("value")) ,
                        "fast_axis_offset" : parseFloat(document.getElementById("textFastAxisOffset").getAttribute("value")) ,
                        "slow_axis_amp" : parseFloat(document.getElementById("textSlowAxisAmplitude").getAttribute("value")) ,
                        "slow_axis_offset" : parseFloat(document.getElementById("textSlowAxisOffset").getAttribute("value")) };
            socket.send(JSON.stringify(json));
        }

        function playButtonClick(){
            var _json = { "request" : "run" };
            g_currentFrame = 0;
            socket.send(JSON.stringify(_json));
        }


        socket.onopen = () => {
            socketOpen = 1;
            var j = { "request" : "get_daq" };
            socket.send(JSON.stringify(j));
        }
        
        socket.onmessage = function (event) {
            if(event.data instanceof Blob){
                switch(lastMessage["response"]){
                    case "ascans":
                        var fileReader = new FileReader();
                            fileReader.onload = (event) => {
                                ascans = new Float64Array(event.target.result);
                                raw = ascans.subarray(0, g_RawAScanLength);
                                inten = ascans.subarray(g_RawAScanLength, g_RawAScanLength + g_IntAScanLength);                                
                                Plotly.restyle('canvasRawAScan', 'y', [raw]);
                                Plotly.restyle('canvasIntensityAScan', 'y', [inten]);
                            };
                        fileReader.readAsArrayBuffer(event.data);
                        break;

                    case "frame":
                        var fileReader = new FileReader();
                        fileReader.onload = (event) => {
                            g_ProcessingFrame = 1;
                            g_LastBScan = event.target.result;
                            var c = document.getElementById("BScanCanvas");
                            var width = g_AScanPerBScan;
                            var height = g_PointsPerAScan;
                            var uint8 = new Uint8Array(g_LastBScan);
                            var imageDataArray = new Uint8ClampedArray(height*width*4);
                            for(var i = 0; i < height; i++){
                                for(var j = 0; j < width; j++){
                                    var tmp = m_LUT[uint8[j*height + i]];
                                    var out = 4*i*width + 4*j;
                                    imageDataArray[out + 2] = tmp & 0xFF;
                                    imageDataArray[out + 1] = (tmp >> 8) & 0xFF;
                                    imageDataArray[out] = (tmp >> 16) & 0xFF;
                                    imageDataArray[out+ 3] = 255;       
                                }                         
                            }
                            var image = new ImageData(imageDataArray, width, height);
                            var ctx = c.getContext("2d");

                            //Render to offscreen canvas
                            var offScreenCanvas = document.createElement('canvas');
                            offScreenCanvas.width = width;
                            offScreenCanvas.height = height;
                            var offscreenContext = offScreenCanvas.getContext("2d");
                            offscreenContext.putImageData(image, 0, 0);

                            var w_ratio = c.width / width;
                            var h_ratio = c.height / height;

                            ctx.save();
                            //ctx.scale(c.width / w, h / c.height);
                            //ctx.translate(c.width / 2, c.height / 2);
                            //ctx.rotate(90.0 * Math.PI / 180);
                            ctx.drawImage(offScreenCanvas, 0, 0, 1024, height*h_ratio);
                            ctx.restore();

                            ctx.beginPath();
                            ctx.moveTo(0, g_StartEnFace);
                            ctx.lineTo(bscanCanvas.width, g_StartEnFace);
                            ctx.lineWidth = 2;
                            ctx.strokeStyle = '#FFFF00';
                            ctx.stroke();

                            ctx.beginPath();
                            ctx.moveTo(0, g_EndEnFace);
                            ctx.lineTo(bscanCanvas.width, g_EndEnFace);
                            ctx.stroke();

                            var tmp_ascan = (g_AScanPerBScan - g_SelectedAScan)/g_AScanPerBScan;
                            ctx.beginPath();
                            ctx.moveTo(tmp_ascan, 0);
                            ctx.lineTo(tmp_ascan, bscanCanvas.height);
                            ctx.stroke();
                            g_ProcessingFrame = 0;                            
                        };
                        fileReader.readAsArrayBuffer(event.data);
                    break;

                    case "enface":
                        var fileReader = new FileReader();
                        if(g_numberOfFrames == 1){
                            var c = document.getElementById("EnFaceCanvas");
                            var ctx = c.getContext("2d");
                            ctx.clearRect(0, 0, c.width, c.height);
                        }else{
                            fileReader.onload = (event) => {
                                g_LastEnFace = event.target.result;
                                var c = document.getElementById("EnFaceCanvas");
                                var width = g_AScanPerBScan;
                                var height = g_numberOfFrames;
                                var uint8 = new Uint8Array(g_LastEnFace);
                                var imageDataArray = new Uint8ClampedArray(height*width*4);
                                for(var i = 0; i < height*width; i++){
                                    var tmp = m_LUT[uint8[i]];
                                    imageDataArray[4*i + 2] = tmp & 0xFF;
                                    imageDataArray[4*i + 1] = (tmp >> 8) & 0xFF;
                                    imageDataArray[4*i] = (tmp >> 16) & 0xFF;
                                    imageDataArray[4*i + 3] = 255;
                                }
                                var image = new ImageData(imageDataArray, width, height);
                                var ctx = c.getContext("2d");

                                //Render to offscreen canvas
                                var offScreenCanvas = document.createElement('canvas');
                                offScreenCanvas.width = c.width;
                                offScreenCanvas.height = c.height;
                                var offscreenContext = offScreenCanvas.getContext("2d");
                                offscreenContext.putImageData(image, 0, 0);

                                ctx.drawImage(offScreenCanvas, 
                                        0, 0, width, height, 
                                        0, 0, c.width, c.height);  
                                        
                                let cssCanvas = window.getComputedStyle(c);
                                let h = parseInt(cssCanvas.getPropertyValue("height"), 10);
                                var tmp = h*g_LastProcessedFrame/g_numberOfFrames
                                

                                ctx.beginPath();
                                ctx.moveTo(0, tmp);
                                ctx.lineTo(bscanCanvas.width, tmp);
                                ctx.lineWidth = 2;
                                ctx.strokeStyle = '#FFFF00';
                                ctx.stroke();
                            }      
                            fileReader.readAsArrayBuffer(event.data);   
                        }           
                    break;
                }

            }else{
                lastMessage = JSON.parse(event.data);
                switch(lastMessage["response"]){
                    case "frame":
                        g_LastProcessedFrame = lastMessage["frame"];
                        break;

                    case "get_daq":
                        g_AScanPerBScan = lastMessage["width"];
                        g_PointsPerAScan = lastMessage["height"];
                        g_numberOfFrames = lastMessage["frames"];
                        g_RawAScanLength = lastMessage["ascan_length"];
                        g_IntAScanLength = lastMessage["fft_length"];
                        g_EndEnFace = g_PointsPerAScan/2;
                        g_ServerMode = lastMessage["mode"];

                        if(g_ServerMode === "daq"){
                            document.getElementById("labelPlayAcquire").innerHTML = "Acquire";
                            handle_FrameRequester = setInterval(() => {
                                if(g_ProcessingFrame == 0){
                                    var _json = { "request" : "frame", "frame" : 0 };
                                    socket.send(JSON.stringify(_json));
                                }
                            }, 50);
                            g_ProcessingFrame = 0;
                        }else{
                            document.getElementById("labelPlayAcquire").innerHTML = "Play";
                            clearInterval(handle_FrameRequester);
                        }

                        var tmp_raw_labels = Array(g_RawAScanLength);
                        var tmp_raw_tmp = Array(g_RawAScanLength);
                        for(var i = 0; i < tmp_raw_labels.length; i++){
                            tmp_raw_labels[i] = i;
                            tmp_raw_tmp[i] = 0;
                        }

                        var trace1 = {
                            x: tmp_raw_labels,
                            y: tmp_raw_tmp,
                            type: "scatter",
                        }

                        var ele = document.getElementById('canvasRawAScan');
                        var config = {
                            title: 'Raw A-Scan',
                        };

                        Plotly.newPlot(ele, [trace1], config);
                        // window.rawLineChart = new Chart(ctx, rawConfig);

                        tmp_raw_labels = Array(g_IntAScanLength);
                        tmp_raw_tmp = Array(g_IntAScanLength);
                        for(var i = 0; i < tmp_raw_labels.length; i++){
                            tmp_raw_labels[i] = i;
                            tmp_raw_tmp[i] = 0;
                        }

                        var trace1 = {
                            x: tmp_raw_labels,
                            y: tmp_raw_tmp,
                            type: "scatter",
                        }

                        ele = document.getElementById('canvasIntensityAScan');
                        config = {
                            title: 'Intensity A-Scan',
                        };

                        Plotly.newPlot(ele, [trace1], config);

                        break;

                    case "load":
                        g_numberOfFrames = lastMessage["frames"];
                        sliderBScanSelect.noUiSlider.updateOptions({
                            range: {
                                'min': 0,
                                'max': g_numberOfFrames 
                            }
                        });
                        break;

                    case "ascans":
                        break;

                    default:
                        break;
                }
            }
        }