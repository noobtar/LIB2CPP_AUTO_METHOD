#include <iostream> 
#include <dlfcn.h>
#include <android/log.h>
#include <unistd.h>

#define targetLib "libil2cpp.so"


	void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
	void *(*il2cpp_domain_get)();
	
	
    void *(*il2cpp_class_from_name)(const void *image, const char *namespaze, const char *name);
    void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name);
	void (*il2cpp_field_static_get_value)(void *field, void *value);
    void (*il2cpp_field_static_set_value)(void *field, void *value);
	size_t (*il2cpp_field_get_offset)(void *field);
 
    
    const void *(*il2cpp_assembly_get_image)(const void *assembly);  
    const char *(*il2cpp_image_get_name)(void *image);
    void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name, int argsCount);
    int (*il2cpp_image_get_class_count)(void *image_ptr);
    void* (*il2cpp_image_get_class)(void* image_ptr, int index);
    const char* (*il2cpp_class_get_namespace)(void* klass);
   const char* (*il2cpp_class_get_name)(void* klass);
   void** (*il2cpp_class_get_methods)(void* klass, void** iter);
   const char* (*il2cpp_method_get_name)(void* method);
   

const int field = 0;

void* TARIL2CppGetMethodoffset(const char* tar_image, const char* tar_namespaze, const char* tar_clazz,const char* tar_method,const int argCount) {
  
    size_t size = 0;
  
    void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);

    if (!assemblies){
        
    for (int i = 0; i < size; ++i) {
        
    void *img = (void *)il2cpp_assembly_get_image(assemblies[i]);
        
   const char *img_name = il2cpp_image_get_name(img);

    
    if (strcmp(img_name,tar_image) == field && img != nullptr) { // เปรียบเทียบ DLL
    
    int index_class =  il2cpp_image_get_class_count(img);
    
    for(int index = 0;index < index_class;++index){
  
    void* ptr_clazz =  il2cpp_image_get_class(img,index);
    
    const char* get_namespace_of_class =  il2cpp_class_get_namespace(ptr_clazz);
    
    const char* get_all_clazz = il2cpp_class_get_name(ptr_clazz);
        
    
    if (strcmp(get_all_clazz,tar_clazz) == field && strcmp(get_namespace_of_class,tar_namespaze) == field) { // เปรียบเี่ยบ Class และ namespace
        
   void* klass = il2cpp_class_from_name(img, get_namespace_of_class, get_all_clazz);
  
   void* iter = nullptr;
   void** method = nullptr;

   while ((method = (void**)il2cpp_class_get_methods(klass, &iter)) != nullptr) {
   const char* method_name = il2cpp_method_get_name(method);
   
    if (strcmp(method_name,tar_method) == field) { // เปรียบเทียบ method
  
    void **method_field_arg = (void**)il2cpp_class_get_method_from_name(klass,method_name,argCount); //getpoint function 
    
    if(method_field_arg != nullptr){
    
    __android_log_print(ANDROID_LOG_ERROR, "METHOD", "%s:%s:%s:%s : %p",img_name,get_namespace_of_class,get_all_clazz,method_name,*method_field_arg);

    return *method_field_arg; //ส่งคืน pointer fonction method
 
    }else{
        
       __android_log_print(ANDROID_LOG_ERROR, "METHOD", "ไม่พบ or nullptr or ใส่ อากิวเม้นผิด");
 
        return nullptr;
    }
    
  } }  }
       } 
       }// dll 
    
   } //loop dll
    
    return nullptr;
    
     }
}


void TARIL2CppAttach() {
    
	void *IL2Cpp_Handle = nullptr;
	
    
	while (!IL2Cpp_Handle) {
		IL2Cpp_Handle = dlopen(targetLib, 4);
		sleep(1);
	}
    
	
	il2cpp_domain_get_assemblies = (void **(*)(const void *, size_t *)) dlsym(IL2Cpp_Handle, "il2cpp_domain_get_assemblies");
	il2cpp_domain_get = (void *(*)()) dlsym(IL2Cpp_Handle, "il2cpp_domain_get");
    il2cpp_assembly_get_image = (const void *(*)(const void *)) dlsym(IL2Cpp_Handle, "il2cpp_assembly_get_image");
	il2cpp_image_get_name = (const char *(*)(void *)) dlsym(IL2Cpp_Handle, "il2cpp_image_get_name");
    il2cpp_class_from_name = (void* (*)(const void*, const char*, const char *)) dlsym(IL2Cpp_Handle, "il2cpp_class_from_name");
    il2cpp_class_get_field_from_name = (void* (*)(void*, const char *)) dlsym(IL2Cpp_Handle, "il2cpp_class_get_field_from_name");
    il2cpp_field_static_get_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, "il2cpp_field_static_get_value");
	il2cpp_field_static_set_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, "il2cpp_field_static_set_value");
    il2cpp_class_get_method_from_name = (void* (*)(void *, const char*, int)) dlsym(IL2Cpp_Handle, "il2cpp_class_get_method_from_name");
    il2cpp_field_get_offset = (size_t (*)(void *)) dlsym(IL2Cpp_Handle, "il2cpp_field_get_offset");
 
    il2cpp_image_get_class_count = (int (*)(void*))  dlsym(IL2Cpp_Handle, "il2cpp_image_get_class_count");
   il2cpp_image_get_class = (void* (*)(void*, int)) dlsym(IL2Cpp_Handle, "il2cpp_image_get_class");
    il2cpp_class_get_namespace = (const char* (*)(void*)) dlsym(IL2Cpp_Handle, "il2cpp_class_get_namespace");
    il2cpp_class_get_name = (const char* (*)(void*))  dlsym(IL2Cpp_Handle, "il2cpp_class_get_name");
    il2cpp_class_get_methods = (void** (*)(void*, void**)) dlsym(IL2Cpp_Handle, "il2cpp_class_get_methods");
   il2cpp_method_get_name = (const char* (*)(void*)) dlsym(IL2Cpp_Handle, "il2cpp_method_get_name");
   
   dlclose(IL2Cpp_Handle);
}
