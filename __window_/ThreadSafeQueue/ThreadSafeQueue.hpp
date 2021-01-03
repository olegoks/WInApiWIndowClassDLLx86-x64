#ifndef THREADSAFEQUEUE_HPP
#define THREADSAFEQUEUE_HPP

#include <memory>
using std::shared_ptr;
using std::make_shared;

//This template class is thread safe when one thread is Push data and second is POp data.
//Stored data have to have copying operator= and default constructor
template<class DataType>
class ThreadSafeQueue {
private:

    class Element final {
    public:

        DataType data_;
        shared_ptr<Element> next_;

        explicit Element(const DataType& data = DataType{})noexcept(true) :
            data_{ data }, next_{ nullptr } {};

    };

    shared_ptr<Element> tail_;
    shared_ptr<Element> head_;

public:

    explicit ThreadSafeQueue()noexcept(true);
    bool Empty()const noexcept(true);
    DataType Pop()noexcept(false);
    void Push(const DataType& data)noexcept(true);

};

template<class DataType>
ThreadSafeQueue<DataType>::ThreadSafeQueue()noexcept(true) :
    tail_{ nullptr },
    head_{ nullptr }{

    tail_ = make_shared<Element>();
    head_ = tail_;

}

template<class DataType>
bool ThreadSafeQueue<DataType>::Empty()const noexcept(true) {

    return (tail_ == head_);

}

template<class DataType>
DataType ThreadSafeQueue<DataType>::Pop()noexcept(false) {

    //if queue is emoty return dafault object
    if (Empty()) return DataType{};

    shared_ptr<Element> ptr = head_;

    //loop to pre-last element
    while (ptr->next_ != tail_)
        ptr = ptr->next_;

    //tail is reference to pre-last element
    tail_ = ptr;

    //ptr element will be deleted ~shared_ptr()
    return ptr->next_->data_;
}

template<class DataType>
void ThreadSafeQueue<DataType>::Push(const DataType& data)noexcept(true) {

    shared_ptr<Element> ptr = head_;

    //Have pointer to the last element
    while (ptr != tail_)
        ptr = ptr->next_;

    //Create new element and save pointer to this
    ptr->next_ = make_shared<Element>(data);

    //Relocate tail pointer to last(added element)
    tail_ = ptr->next_;

}

#endif //THREADSAFEQUEUE_HPP