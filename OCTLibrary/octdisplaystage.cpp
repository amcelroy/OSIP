#include "octdisplaystage.h"

using namespace OSIP;

static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
	std::vector<unsigned char> *p = (std::vector<unsigned char>*)png_get_io_ptr(png_ptr);
	p->insert(p->end(), data, data + length);
}

void OCTDisplayStage::preStage(){

}


void OCTDisplayStage::_writePNG(png_structp png, png_infop info, int w, int h, vector<unsigned char>& scaledImage, vector<unsigned char> *pngBuffer) {
	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info = png_create_info_struct(png);

	png_set_compression_level(png, 6);
	png_set_IHDR(png,
		info,
		w,
		h,
		8,
		PNG_COLOR_TYPE_GRAY,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);


	pngBuffer->clear();
	std::vector<unsigned char*> rows(h);
	for (size_t y = 0; y < h; ++y)
		rows[y] = (unsigned char*)scaledImage.data() + y * w;

	png_set_rows(png, info, &rows[0]);
	png_set_write_fn(png, pngBuffer, PngWriteCallback, NULL);
	png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);
	png_write_flush(png);

	if (info != NULL) png_free_data(png, info, PNG_FREE_ALL, -1);
	if (png != NULL) png_destroy_write_struct(&png, (png_infopp)NULL);
}

void OCTDisplayStage::work(){

	int b_scan_update_counter = 0;
	int en_face_update_counter = 0;

    while(!this->stopThread){
        if(this->pauseThread){
            pipelineSleep(5);
        }else{
            Payload<float> p = this->fetchPayload();

            if(!p.isValid()){
                this->pipelineSleep(1);
            }else{
                try{
                    auto start = chrono::high_resolution_clock::now();

                    vector<vector<unsigned long long>> dims = p.getDimensions();
                    vector<shared_ptr<vector<float>>> datas = p.getData();

                    

                    //dims: 0 - A per B, 1 - Total B, 2 - Current B
                    auto enFace = p.findByDataName("EnFace_Slice");
                    vector<unsigned long long> enFaceDims = get<1>(enFace);
                    shared_ptr<vector<float>> enFaceData = get<0>(enFace);

                    unsigned long long currentFrame = dims[0][2];

                    unsigned long arraySize = dims[0][0]*dims[0][1];


                    if(m_bscan_8bit.size() != arraySize){
                        m_bscan_8bit = vector<unsigned char>(arraySize);
                    }

					if (b_scan_update_counter >= 8) {
						scaleTo8Bit(*(datas.at(0).get()), &m_bscan_8bit);
						m_BScanAccessMutex.lock();
						_writePNG(m_bscan_ptr, m_bscan_info_ptr, dims[0][0], dims[0][1], m_bscan_8bit, &m_bscan_png);
						m_BScanAccessMutex.unlock();
						b_scan_update_counter = 0;
					}
					else {
						b_scan_update_counter += 1;
					}

                    vector<unsigned char> tmp_slice(dims[0][1]);
                    scaleTo8Bit(*(enFaceData.get()), &tmp_slice);
                    memcpy(&m_enface_8bit.data()[currentFrame*dims[0][1]], tmp_slice.data(), dims[0][1]);					
					if (en_face_update_counter >= 16) {
						m_EnFaceAccessMutex.lock();
						_writePNG(m_enface_ptr, m_enface_info_ptr, dims[2][0], dims[2][1], m_enface_8bit, &m_enface_png);
						m_EnFaceAccessMutex.unlock();
						en_face_update_counter = 0;
					}
					else {
						en_face_update_counter += 1;
					}					

                    if(m_FramesPerSecond != 0.0f){
                        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(1/m_FramesPerSecond)));
                    }

                    auto stop = chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::micro> elapsed = stop - start;
                    this->sig_StageTimer(static_cast<float>(elapsed.count()));
                    d_ThreadWorkTime = elapsed.count();

                    p.finished();
                }catch(...){
                    log("Generic error in OCT Display Stage");
                }
            }
        }
    }
}

void OCTDisplayStage::postStage(){

}

