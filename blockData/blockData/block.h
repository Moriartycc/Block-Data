#ifndef _BLOCK_H
#define _BLOCK_H
#define BLOCK_DEFAULT_SIZE 100
#define SINGLE_BLOCK_EXPANSION_RATE 2

namespace blockSignal {
	enum signal { front_succeed, back_succeed, front_failed, back_failed, erase_failed, empty };
}

template <class T>
class blockList;

template <class T>
class block {
public:
	typedef T value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
private:
	int blockSize;
	iterator blockData, storageStart, storageFinish, start, finish;
public:
	iterator begin() { return start; }
	iterator end() { return finish; }
	iterator middle() { return (start + this->size() / 2); }
	value_type& front() { return *start; }
	value_type& back() { return *(finish - 1); }
	const_iterator begin() const { return start; }
	const_iterator end() const { return end; }
	int size() const { return (finish - start); }
	int storageSize() const { return blockSize; }
	block();
	block(int);
	block(int, iterator, iterator);
	block(const block<T> &);
	block(block<T> *const);
	~block();
	block<T>& operator=(const block<T> &);
	T& operator[](int i);
	blockSignal::signal insert(iterator, const value_type &);
	blockSignal::signal insert(iterator, iterator, iterator);
	blockSignal::signal erase(iterator);
	blockSignal::signal erase(iterator, iterator);
	bool empty() { return (start == finish); }
	blockSignal::signal push_back(const value_type &val) { return insert(finish, val); }
	blockSignal::signal push_front(const value_type &val) { return insert(start, val); }
	blockSignal::signal pop_back(iterator position) { return erase(finish - 1); }
	blockSignal::signal pop_front(iterator position) { return erase(start); }
	void clear();
private:
	iterator _auto_insert(iterator, const value_type &);
	void _auto_push_back(const value_type &val);
	void _auto_push_front(const value_type &val);
	void _auto_expand();
	template <class X> friend class blockList;
};

//block成员函数定义
template <class T>
block<T>::block() : blockSize(BLOCK_DEFAULT_SIZE) {
	blockData = new value_type[blockSize];
	storageStart = blockData;
	storageFinish = blockData + blockSize;
	start = finish = blockData + blockSize / 2;
}

template <class T>
block<T>::block(int _blockSize) : blockSize(_blockSize) {
	blockData = new value_type[blockSize];
	storageStart = blockData;
	storageFinish = blockData + blockSize;
	start = finish = blockData + blockSize / 2;
}

template <class T>
block<T>::block(int _blockSize, iterator valStart, iterator valFinish) : blockSize(_blockSize) {
	int valSize;
	if (valFinish) valSize = valFinish - valStart; else valSize = 1;
	blockData = new value_type[blockSize];
	storageStart = blockData;
	storageFinish = blockData + blockSize;
	start = blockData + blockSize / 2 - valSize / 2;
	finish = start + valSize;
	for (iterator i = start; i < finish; i++) *i = *(valStart + (i - start));
}

template <class T>
block<T>::block(const block<T> &_block) : blockSize(_block.size()) {
	blockData = new value_type[blockSize];
	storageStart = blockData;
	storageFinish = blockData + blockSize;
	start = storageStart + (_block.start - _block.storageStart);
	finish = storageStart + (_block.finish - _block.storageStart);
	for (iterator i = start; i < finish; i++) *i = *(_block.start + (i - start));
}

template <class T>
block<T>::block(block<T> *const _block) : blockSize(_block->size()) {
	blockData = new value_type[blockSize];
	storageStart = blockData;
	storageFinish = blockData + blockSize;
	start = storageStart + (_block->start - _block->storageStart);
	finish = storageStart + (_block->finish - _block->storageStart);
	for (iterator i = start; i < finish; i++) *i = *(_block->start + (i - start));
}

template <class T>
block<T>::~block() {
	if (blockData) delete[] blockData;
}

template <class T>
block<T>& block<T>::operator=(const block<T> &_block) {
	if (blockData) delete[] blockData;
	blockSize = _block.size();
	blockData = new value_type[blockSize];
	storageStart = blockData;
	storageFinish = blockData + blockSize;
	start = storageStart + (_block.start - _block.storageStart);
	finish = storageStart + (_block.finish - _block.storageStart);
	for (iterator i = start; i < finish; i++) *i = *(_block.start + (i - start));
	return (*this);
}

template <class T>
T& block<T>::operator[](int i) {
	return (*(start + i));
}

template <class T>
blockSignal::signal block<T>::insert(iterator position, const value_type &val) {
	if (position - start < finish - position) {
		if (start == storageStart) return blockSignal::front_failed;
		for (iterator i = start - 1; i < position - 1; i++) *i = *(i + 1);
		*(position - 1) = val;
		start--;
		return blockSignal::front_succeed;
	}
	else {
		if (finish == storageFinish) return blockSignal::back_failed;
		for (iterator i = finish; i > position; i--) *i = *(i - 1);
		*position = val;
		finish++;
		return blockSignal::back_succeed;
	}
}

template <class T>
blockSignal::signal block<T>::insert(iterator position, iterator valStart, iterator valFinish) {
	int valSize = valFinish - valStart;
	if (position - start < finish - position) {
		if (start - storageStart < valSize) return blockSignal::front_failed;
		for (iterator i = start - valSize; i < position - valSize; i++) *i = *(i + valSize);
		for (iterator i = valStart; i < valFinish; i++) *(position - valSize + (i - valStart)) = *i;
		start = start - valSize;
		return blockSignal::front_succeed;
	}
	else {
		if (storageFinish - finish < valSize) return blockSignal::back_failed;
		for (iterator i = finish + valSize - 1; i > position + valSize - 1; i--) *i = *(i - valSize);
		for (iterator i = valStart; i < valFinish; i++) *(position + (i - valStart)) = *i;
		finish = finish + valSize;
		return blockSignal::back_succeed;
	}
}

template <class T>
blockSignal::signal block<T>::erase(iterator position) {
	if (position >= finish || position < start) return blockSignal::erase_failed;
	if (start - storageStart < storageFinish - finish) {
		for (iterator i = position; i > start; i--) *i = *(i - 1);
		start++;
		if (start == finish) return blockSignal::empty; else return blockSignal::front_succeed;
	}
	else {
		for (iterator i = position; i < finish - 1; i++) *i = *(i + 1);
		finish--;
		if (start == finish) return blockSignal::empty; else return blockSignal::back_succeed;
	}
}

template <class T>
blockSignal::signal block<T>::erase(iterator eraseStart, iterator eraseFinish) {
	if (eraseFinish > finish || eraseStart < start || eraseStart >= eraseFinish) return blockSignal::erase_failed;
	int eraseSize = eraseFinish - eraseStart;
	if (start - storageStart < storageFinish - finish) {
		for (iterator i = eraseFinish - 1; i >= start + eraseSize; i--) *i = *(i - eraseSize);
		start = start + eraseSize;
		if (start == finish) return blockSignal::empty; else return blockSignal::front_succeed;
	}
	else {
		for (iterator i = eraseStart; i < finish - eraseSize; i++) *i = *(i + eraseSize);
		finish = finish - eraseSize;
		if (start == finish) return blockSignal::empty; else return blockSignal::back_succeed;
	}
}

template <class T>
void block<T>::clear() {
	start = finish = blockData + blockSize / 2;
}

//返回被插入的数原来位置上的数的位置，方便blockList操作
template <class T>
typename block<T>::iterator block<T>::_auto_insert(iterator position, const value_type &val) {
	blockSignal::signal _signal = this->insert(position, val);
	int i;
	switch(_signal) {
	case blockSignal::front_failed:
	case blockSignal::back_failed:
		i = position - this->begin();
		this->_auto_expand();
		switch (this->insert(this->begin() + i, val)) {
		case blockSignal::front_succeed:
			return this->begin() + i + 1;
			break;
		case blockSignal::back_succeed:
			return this->begin() + i + 1;
			break;
		}
		break;
	case blockSignal::front_succeed:
		return position;
		break;
	case blockSignal::back_succeed:
		return position + 1;
		break;
	}
	return position;
}

template <class T>
void block<T>::_auto_push_back(const value_type &val) {
	blockSignal::signal _signal = this->push_back(val);
	if (_signal == blockSignal::front_failed || _signal == blockSignal::back_failed) {
		this->_auto_expand();
		this->push_back(val);
	}
}

template <class T>
void block<T>::_auto_push_front(const value_type &val) {
	blockSignal::signal _signal = this->push_front(val);
	if (_signal == blockSignal::front_failed || _signal == blockSignal::back_failed) {
		this->_auto_expand();
		this->push_front(val);
	}
}

template <class T>
void block<T>::_auto_expand() {
	iterator _blockData = new value_type[SINGLE_BLOCK_EXPANSION_RATE * blockSize];
	iterator _start = _blockData + SINGLE_BLOCK_EXPANSION_RATE * blockSize / 2 - this->size() / 2;
	for (iterator i = start; i < finish; i++) *(_start + (i - start)) = *i;
	iterator _finish = _start + (finish - start);
	delete[] blockData;
	blockData = _blockData;
	storageStart = blockData;
	storageFinish = blockData + SINGLE_BLOCK_EXPANSION_RATE * blockSize;
	start = _start;
	finish = _finish;
	blockSize = SINGLE_BLOCK_EXPANSION_RATE * blockSize;
}
#endif
