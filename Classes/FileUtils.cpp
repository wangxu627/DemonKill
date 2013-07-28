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

#include "FileUtils.h"
#include <stdio.h>

class FileImpl
{
public:
	FileImpl(const char* name,const char* mode)
	{
		mFS = fopen(name,mode);
	}

	int read(char* output,int n)
	{
		if(!mFS)
		{
			return 0;
		}
		return fread(output,1,n,mFS);
	}

	int write(void* data,int len)
	{
		if(!mFS)
		{
			return 0;
		}
		return fwrite(data,1,len,mFS);
	}

	void close()
	{
		if(!mFS)
		{
			return;
		}
		fclose(mFS);
	}

	int state()
	{
		return mFS == 0 ? 1 : 0;
	}
private:
	FILE* mFS;
};




FileUtils::FileUtils(){}

FileUtils::FileUtils(const char* name,const char* mode)
			  :_pimpl(new FileImpl(name,mode))
{}

int FileUtils::read(char* output,int n)
{
	return _pimpl->read(output,n);
}

int FileUtils::write( void* data,int len )
{
	return _pimpl->write(data,len);
}

void FileUtils::close()
{
	_pimpl->close();
}

int FileUtils::state()
{
	return _pimpl->state();
}
