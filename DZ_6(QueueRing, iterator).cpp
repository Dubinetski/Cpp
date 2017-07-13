/*
Написать класс "циклической очереди" RingQueue<T>
с хранением данных в сплошной непреровной памяти (как у вектора),
реализовать функции добавления, удаления, итерирования по всем элементам.
Напишите код, который будет проверять правильность работы вашего кода.
*/

 /* В отличие от примера, приведенного в теории к уроку №7 удалось сделать сложность вставки О(1) вместо О(n) */


#include <iostream>
#include <algorithm>

template<class T>
class RingQueue {
    T* ring;
    int capacity;
    int size;
    int _begin;
    int _end;

    /*следующий элемент очереди*/
    int next(int& position) {
        while(position >= capacity - 1)
            position -= capacity;
        return position + 1;
    }
    /*предыдущий элемент в очереди*/
    int prev(int& position) {
        return (position == 0) ? capacity - 1 : position - 1;
    }

    public:
    RingQueue<T>(int n) {
        capacity = n;
        ring = new T[capacity];
        size = 0;
        _begin = 0;
        _end = 0;
    }

    RingQueue<T>() { RingQueue<T>(10); }

    ~RingQueue<T>() {
        delete[] ring;
    }

    bool isEmpty() { return !size; }
    bool isFull() { return size == capacity; }
    /* Возвращает колличество элементов в очереди */
    int getCount() { return size; }
    /* Возвращает максимальный размер очереди */
    int get_capacity() { return capacity; }

    /* Добавление нового элемента в конец очереди */
    void push(const T& elem) {
        if(!isFull()) {
            ring[_end] = elem;
            _end = next(_end);
            ++size;
        }
    }

    /* Извлечение элемента с помещением его в конец очереди */
    T& extruct() {
        if(!isEmpty()) {
            T& tmp = front();
            if(!isFull()) {
                push(tmp);
                _begin = next(_begin);
                --size;
            } else {
                _begin = next(_begin);
                _end = next(_end);
            }
            return tmp;
        }
    }

    /* Удаление первого элемента */
    void pop() {
        if(size) {
            --size;
            _begin = next(_begin);
        }
    }

    /* Возвращает ссылку на первый элемент очереди */
    T& front() {
        if(!isEmpty())
            return ring[_begin];
    }

    /* Возвращает ссылку на последний элемент очереди */
    T& back() {
        if(!isEmpty())
            return ring[prev(_end)];
    }
        
    /* Очистка очереди */
    void clear() {
        size = 0;
        _begin = 0;
        _end = 0;
    }

    /* Итератор для очереди */
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
        RingQueue<T>* ringQueue;
        int current_pos;
        int flag = -1;

        public:
        iterator(RingQueue<T>* _ring, int position) {
            ringQueue = _ring;
            current_pos = position;
        }
        iterator& operator ++() {
            if(++current_pos == ringQueue->get_capacity())
                current_pos -= ringQueue->get_capacity();
            return *this;
        }
        iterator operator ++(int) { auto old = *this; ++(*this); return old; }
        iterator& operator--() {
            if(--current_pos == -1)
                current_pos += ringQueue->get_capacity();
            return *this;
        }
        iterator operator--(int) { auto old = *this; --(*this); return old; }

        pointer operator ->() const;

        reference& operator *() const { return ringQueue->ring[current_pos]; }
        void swap(iterator& b) { std::swap(**this, *b); }
        bool operator ==(const iterator& rhs) {
            if(&**this == &(*rhs)) {
                if(ringQueue->isFull() && (&**this == &ringQueue->front())) {
                    flag *= -1;
                    return (flag < 0) ? true : false;
                } else {
                    return true;
                }
            } else
                return false;
        }
        bool operator !=(const iterator& rhs) { return !(*this == rhs); }
    };
    
    iterator& begin() {
        iterator iter(this, _begin);
        //std::cout << &ring[0] << "< - begin \n";
        return iter;
    }
    iterator& end() {
        iterator iter(this, _end);
        //std::cout << &ring[size-1] << "< - end \n";
        return iter;
    }
};

void main() {
    RingQueue<double> ring(4);
    std::cout << "Is Empty? (true): " << (ring.isEmpty() ? "true" : "false") << "\n";
    std::cout << "Is Full? (false): " << (ring.isFull() ? "true" : "false") << "\n\n";

    ring.push(5);
    ring.push(4);
    ring.push(3);
    ring.push(2);

    std::cout << "After push elements 5, 4, 3, 2: \n";
    std::cout << "Count (4): " << ring.getCount() << "\n";
    std::cout << "Capacity (4): " << ring.get_capacity() << "\n";
    std::cout << "First elem (5): " << ring.front() << "\n";
    std::cout << "Last elem (2): " << ring.back() << "\n";
    std::cout << "Is Full? (true): " << (ring.isFull() ? "true" : "false") << "\n";
    std::cout << "Iterator from begin to end (5 4 3 2): ";
    auto end = ring.end();
    for(RingQueue<double>::iterator i = ring.begin(); i != end; ++i)
        std::cout << *i << ' ';
    std::cout << "\n\n";

    for(auto i : ring)
        std::cout << i << ' ';
    std::cout << "\n\n";

    ring.pop();
    std::cout << "After pop():\nFirst elem (4): " << ring.front() << "\n";
    std::cout << "Iterator from begin to end (4 3 2): ";
    for(auto i : ring)
        std::cout << i << ' ';
    std::cout << "\n\n";

    std::cout << "extruct() (4):" << ring.extruct() << "\n";
    std::cout << "After extruct():\nFirst elem (3): " << ring.front() << "\n";
    std::cout << "Last elem (4): " << ring.back() << "\n";
    std::cout << "Iterator from begin to end (3 2 4): ";
    for(auto i : ring)
        std::cout << i << ' ';
    std::cout << "\n\n";

    ring.clear();
    std::cout << "After clear(): Is Empty? (true): " << (ring.isEmpty() ? "true" : "false") << "\n";
}