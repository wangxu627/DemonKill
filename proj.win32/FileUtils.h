/* ===================================================== */
//	
//    ®ç®Ä®Ä®Ä®Ä®Ä®é               ®ç®Ä®Ä®Ä®Ä®Ä®Ä®é
// ®ç®è             ®ê®é         ®ç®è             ®ê®é 
// ®è                 ®ê®é     ®ç®è                 ®Ä 
// ®Ü      ®ç®Ä®é       ®ê®é®ç®è       ®ç®Ä®é       ®Ä
// ®é     ®ç®è  ®ê®é               ®ç®é   ®ê®é   ®ç 
// ®ê®Ä®Ä®è   ®ç®è                   ®ê®é   ®ê®Ä®è
//          ®Ä   °Ò               °Ò    ®Ä 
//          ®Ä   °®       ®{       °®   ®Ä 
//           ®ê®é       ®^®m®a       ®ç®è 
//             ®ê®Ä®|®z®z   ®z®z®|®Ä®è 
//               ®ç®è       ®ê®é  
//               ®Ä           ®Ä  
//             ®ç®è®ä       ®Ü®ê®é
//           ®ä®z®é®ç®|®|®|®é®ç®z®Ü
//            
//                 Author : Wangxu
//            Create Date :				
//            Description : 
//
/* ===================================================== */	

#ifndef  __FILEUTILS_H__
#define  __FILEUTILS_H__

class FileImpl;

class FileUtils
{
public:
	FileUtils();

	FileUtils(const char* name,const char* mode);
	
	int read(char* output,int n);

	int write(void* data,int len);

	void close();

private:
	FileImpl* _pimpl;
};;


#endif