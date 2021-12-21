// Circular Queue Template
// When an item is inserted into a full queue, the oldest item is automatically evicted.
#ifndef CIRQUE_H
#define CIRQUE_H

#include <cstring>

template <typename T> class cirque
{
public:
    class index // Like an iterator
    {
    private:
        uint16_t idx;
        uint16_t mask;
        index(int i, int m) : idx(i & m), mask(m) { }

    public:
        index(int QUEUE_SIZE) : idx(0), mask(QUEUE_SIZE-1) { }
        template <typename U> index(const U& u) : idx(u.idx), mask(u.mask) { }
        
        bool operator==(const index& i) const { return idx == i.idx; }
        bool operator!=(const index& i) const { return idx != i.idx; }
        
        index operator+(unsigned int i) const { return index(idx + i, mask); }
        index operator-(unsigned int i) const { return index(idx - i, mask); }
        
        index& operator++()
        { 
            idx = (idx+1) & mask;
            return *this; 
        }
        index operator++(int)
        { 
            index t(idx, mask);
            idx = (idx+1) & mask; 
            return t;
        }
        index& operator--()
        { 
            idx = (idx-1) & mask;
            return *this; 
        }
        index operator--(int)
        { 
            index t(idx, mask);
            idx = (idx-1) & mask; 
            return t;
        }

        index& operator+=(int x)
        { 
            idx = (idx+x) & mask;
            return *this; 
        }
        index& operator-=(int x)
        { 
            idx = (idx-x) & mask;
            return *this; 
        }

        // Really shouldn't use these...
        uint16_t getRawIndex() const { return idx; }
        uint32_t getQueueSize() const { return mask + 1; }
        index makeIndex(uint16_t i) const { return index(i, mask); }

        friend class cirque<T>;
        template<typename U> friend class cirque<U>::index; 
    };

private:
    T* queue;
    index first, next;

public:
    cirque(int QUEUE_SIZE) : first(QUEUE_SIZE), next(QUEUE_SIZE) { queue = new T[QUEUE_SIZE]; }
    cirque(const cirque& c) : first(c.first), next(c.next)
    {
        int QUEUE_SIZE = first.mask + 1; 
        queue = new T[QUEUE_SIZE]; 
        memcpy(queue, c.queue, sizeof(T) * QUEUE_SIZE);
    }
    ~cirque() { delete[] queue; }

    bool empty() const { return first == next; }
    bool full() const { return (next+1) == first; }
    uint32_t size() const { return (next.idx - first.idx) & next.mask; }

    void push_back(const T& x)
    {
        queue[next.idx] = x;
        if(++next == first) pop_front();
    }

    void pop_front() { ++first; }
    void pop_back() { --next; }
    void clear() { first = next; }

    void erase_front(const index& i) // Erase values up to, but not including, index
    { first = i; }

    void erase_back(const index& i) // Erase values after, but not including, index
    { next = i + 1; }

    T& operator[](const index& i) { return queue[i.idx]; }
    const T& operator[](const index& i) const { return queue[i.idx]; }

    T& front() { return queue[first.idx]; }
    const T& front() const { return queue[first.idx]; }

    T& back() { return queue[(next.idx - 1) & next.mask]; }
    const T& back() const { return queue[(next.idx - 1) & next.mask]; }

    const index& begin() const { return first; }
    const index& end() const { return next; }

    // Perform binary search for element.
    // Returns index that is less than or equal to the value being searched for,
    // but where index+1 is greater than the value (or non existant).
    index binary_search(index first, index last, const T& val) const
    {
        while(first.idx != last.idx)
        {
            index mid = first + ((last - first).idx >> 1);

            if(val < queue[mid.idx])
            {
                last = mid;
                continue;
            }

            index after = mid + 1;

            if(after == end() || val < queue[after.idx])
                return mid;

            //first = mid; // which of these is correct?
            first = after;
        }

        return first;
    }
};

#endif
