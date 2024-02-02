#ifndef __COAP_SERVER_SINGLETON_H__
#define __COAP_SERVER_SINGLETON_H__

template<typename T>
class Singleton{
public:
    static T& get_instance() noexcept(std::is_nothrow_constructible<T>::value) {
        static T instance;
        return instance;
    }
    virtual ~Singleton() noexcept{
        std::cout << "destructor called!" << std::endl;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;
protected:
    Singleton() {
        std::cout << "constructor called!" << std::endl;
    }
};

#endif