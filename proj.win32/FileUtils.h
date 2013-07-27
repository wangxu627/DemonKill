/* ===================================================== */
//	
//    ��������������               ����������������
// ����             ����         ����             ���� 
// ��                 ����     ����                 �� 
// ��      ������       ��������       ������       ��
// ��     ����  ����               ����   ����   �� 
// ��������   ����                   ����   ������
//          ��   ��               ��    �� 
//          ��   ��       �{       ��   �� 
//           ����       �^�m�a       ���� 
//             �����|�z�z   �z�z�|���� 
//               ����       ����  
//               ��           ��  
//             ������       ������
//           ���z�����|�|�|�����z��
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