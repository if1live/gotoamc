#include <string.h>	// for memcpy

template <class T>
Queue<T>::Queue(int _capacity)
{
	if(_capacity < 1)
		throw "capacity must be >= 1";
	
	capacity = _capacity;
	mFront = mRear = 0;
	pQueue = new T[capacity];
	pthread_mutex_init( &mutex, NULL );
}

template <class T>
Queue<T>::~Queue<T>()
{
	if(pQueue != 0)
	{
		delete[] pQueue;
		pQueue = NULL;
	}
	pthread_mutex_destroy( &mutex );
}

template <class T>
bool Queue<T>::isEmpty(void)
{
	pthread_mutex_lock( &mutex );
	bool ret = (mFront == mRear);
	pthread_mutex_unlock( &mutex );
		
	return ret;
}


template <class T>
bool Queue<T>::isFull(void)
{
	pthread_mutex_lock( &mutex );
	bool ret = (((mRear+1) % capacity) == mFront);
	pthread_mutex_unlock( &mutex );
	
	return ret;
}


template <class T>
T &Queue<T>::front(void)
{
	pthread_mutex_lock( &mutex );
	bool ret = (mFront == mRear);
	
	if(ret) throw "Queue is empty: No front element";

	T& temp = pQueue[(mFront+1) % capacity];
	pthread_mutex_unlock( &mutex );
   
	return temp;
}

template <class T>
T &Queue<T>::rear(void)
{
	pthread_mutex_lock( &mutex );
	bool ret = (((mRear+1) % capacity) == mFront);
	
	if(ret) throw "Queue is empty: No rear element";

	T& temp = pQueue[mRear];
	pthread_mutex_unlock( &mutex );
   
	return temp;;
}


template <class T>
void Queue<T>::push(T &_item)
{
	pthread_mutex_lock( &mutex );
	if( ((mRear+1) % capacity) == mFront )	// full? then double the capacity
	{
		// DS textbook p.146
		T *newQueue = new T[2*capacity];
		// copy from queue to newQueue
		int start = (mFront + 1) % capacity;
		
		if(start<2)
		{
			// no wrap around
			// copy(queue+start, queue+start+capacity-1, newQueue);
			memcpy(newQueue, pQueue, sizeof(T)*capacity);
		}
		else
		{
			// queue wraps around
			// copy(queue+start, queue+capacity, newQueue);
			// copy(queue, queue+rear+1, newQueue+capacity-start);
			memcpy(newQueue, pQueue, sizeof(T)*(capacity-start+1));
			memcpy(newQueue+capacity-start, pQueue, sizeof(T)*(mRear+2));
		}
		
		// switch to newQueue
		mRear = mFront + capacity - 1;
		capacity = capacity * 2;
		delete[] pQueue;
		pQueue = newQueue;
	}

	mRear = (mRear+1) % capacity;
	pQueue[mRear] = _item;
	pthread_mutex_unlock( &mutex );
}


template <class T>
T Queue<T>::pop()
{
	pthread_mutex_lock( &mutex );
	if( mFront == mRear ) throw "Queue is empty";

	mFront = (mFront + 1)%capacity;
	T temp = pQueue[mFront];
	pthread_mutex_unlock( &mutex );
	return temp;
}

