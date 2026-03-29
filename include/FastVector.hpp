#include <iostream>
#include <stdexcept>
#include <utility>
#include <cstddef>

template <typename T, size_t FixedCapacity>
class FastVector {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

        FastVector() : data_ptr{reinterpret_cast<value_type*>(inline_buffer)}, current_capacity{FixedCapacity}{}
        ~FastVector(){
            clear();
            
            if(is_heap_allocated()){
                ::operator delete(data_ptr);
            }
        }
        FastVector(const FastVector&) = delete;
        FastVector& operator=(const FastVector&) = delete;
        FastVector(FastVector&& other) noexcept : current_size{other.current_size}, current_capacity{other.current_capacity}{
            if(other.is_heap_allocated()){
                data_ptr = other.data_ptr;

                other.data_ptr = reinterpret_cast<value_type*>(other.inline_buffer);
                other.current_size = 0;
                other.current_capacity = FixedCapacity;
            } else {
                data_ptr = reinterpret_cast<value_type*>(inline_buffer);

                for(size_type i = 0; i < current_size; ++i){
                    new (&data_ptr[i]) T(std::move(other.data_ptr[i]));
                    other.data_ptr[i].~T();
                }
                other.current_size = 0;
            }
        }
        FastVector& operator=(FastVector&& other) noexcept {
            if(this == other) {return *this;}
            clear();
            
            if(is_heap_allocated()){
                ::operator delete(data_ptr);
            }
            current_size = other.current_size;
            current_capacity = other.current_capacity;
            if(other.is_heap_allocated()){
                data_ptr = other.data_ptr;

                other.data_ptr = reinterpret_cast<value_type*>(other.inline_buffer);
                other.current_size = 0;
                other.current_capacity = FixedCapacity;
            } else {
                data_ptr = reinterpret_cast<value_type*>(inline_buffer);

                for(size_type i = 0; i < current_size; ++i){
                    new (&data_ptr[i]) T(std::move(other.data_ptr[i]));
                    other.data_ptr[i].~T();
                }
                other.current_size = 0;
            }
        }
        reference at(size_type position) const {
            if(position >= current_size){
                throw std::out_of_range("Position out of bounds of the vector");
            }
            return data_ptr[position];
        }

        reference operator[](size_type position) {
            return data_ptr[position];
        }
        const_reference operator[](size_type position) const{
            return data_ptr[position];
        }
        void clear() noexcept{
            for(size_type i = 0; i < current_size; ++i){
                data_ptr[i].~T();
            }
            current_size = 0;
        }
        size_type size() const noexcept{
            return current_size;
        }
        bool empty() const noexcept {
            if(current_size <= 0) {
                return true;
            }
            return false;
        }
        void pop_back(){
            if (current_size > 0){
                data_ptr[current_size - 1].~T();
                --current_size;
            }
        }
        void pop_front() {
            if (current_size == 0) return;
            data_ptr[0].~T();
            for (std::size_t i = 1; i < current_size; ++i) {
                new (&data_ptr[i - 1]) T(std::move(data_ptr[i]));
                data_ptr[i].~T();
            }
            --current_size;
        }
        void push_back(const_reference item){
            if(current_size >= current_capacity){
                grow();
            }
            new (&data_ptr[current_size]) T(item);
            current_size++; 
        }
        template <typename... Args>
        void emplace_back(Args&&... args){
            if(current_size >= current_capacity){
                grow();
            }
            new (&data_ptr[current_size]) T(std::forward<Args>(args)...);
            current_size++;
        }
    private:
        bool is_heap_allocated() const {
            return data_ptr != reinterpret_cast<const value_type*>(inline_buffer);
        }

        void grow() {
            size_type new_capacity = current_capacity ? current_capacity*2 : 1;
            value_type* new_ptr = static_cast<value_type*>(::operator new(new_capacity * sizeof(T)));
            
            for(size_type i = 0; i < current_size; ++i) {
                new (&new_ptr[i]) T(std::move(data_ptr[i]));
                data_ptr[i].~T();
            }

            if(is_heap_allocated()){
                ::operator delete(data_ptr);
            }

            data_ptr = new_ptr;
            current_capacity = new_capacity;
        }
        value_type* data_ptr;
        size_type current_size = 0;
        size_type current_capacity;

        alignas(value_type) char inline_buffer[FixedCapacity * sizeof(value_type)];
};