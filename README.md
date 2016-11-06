# </center>BlockData项目文档</center>

## 简介

设计模板类<font color = Orchid>blockList</font>实现了大规模数据管理操作，并实现了其随机迭代器。为了实现分块管理，还实现了模板类<font color = Orchid>block</font>

## 文件

- <font color = Navy>block.h</font>
- <font color = Navy>blockList.h</font>

## 主要数据结构和API

### block.h

* blockSignal<font face = consolas size = 2>

  ```c++
  namespace blockSignal {
  	enum signal { front_succeed, back_succeed, front_failed, back_failed, erase_failed, empty };
  }
  ```

  </font>用于表示对block类进行插入、删除操作时的返回值

  * <font color = LightSeaGreen face = consolas>front_succeed</font>

    表示insert操作，push_back操作，push_front操作后**前半部分数据块前移成功**；表示erase操作，pop_front操作，pop_back操作后**前半部分数据块后移成功**

  * <font color = LightSeaGreen face = consolas>back_succeed</font>

    表示insert操作，push_back操作，push_front操作后**后半部分数据块后移成功**；表示erase操作，pop_front操作，pop_back操作后**后半部分数据块前移成功**

  * <font color = LightSeaGreen face = consolas>front_failed</font>

    表示insert操作，push_back操作，push_front操作后**前半部分数据块前移失败**

  * <font color = LightSeaGreen face = consolas>back_failed</font>

    表示insert操作，push_back操作，push_front操作后**后半部分数据块后移失败**

  * <font color = LightSeaGreen face = consolas>erase_failed</font>

    表示erase操作，pop_front操作，pop_back操作失败。

  * <font color = LightSeaGreen face = consolas>empty</font>

    表示erase操作，pop_front操作，pop_back**操作成功且操作完后block为空**

* class block<font face = consolas size = 2>

  ```c++
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
  	const_iterator end() const { return end; } //几种迭代器
  	int size() const { return (finish - start); }
  	int storageSize() const { return blockSize; } 
  	block();
  	block(int);
  	block(int, iterator, iterator);
  	block(const block<T> &);
  	block(block<T> *const); //构造函数
  	~block();
  	block<T>& operator=(const block<T> &);
  	T& operator[](int i); //取值
  	blockSignal::signal insert(iterator, const value_type &);
  	blockSignal::signal insert(iterator, iterator, iterator);
  	blockSignal::signal erase(iterator);
  	blockSignal::signal erase(iterator, iterator);
  	bool empty() { return (start == finish); }
  	blockSignal::signal push_back(const value_type &val) { return insert(finish, val); } //基于insert
  	blockSignal::signal push_front(const value_type &val) { return insert(start, val); } //基于insert
  	blockSignal::signal pop_back(iterator position) { return erase(finish - 1); } //基于erase
  	blockSignal::signal pop_front(iterator position) { return erase(start); } //基于erase
  	void clear();
  private:
  	iterator _auto_insert(iterator, const value_type &);
  	void _auto_push_back(const value_type &val);
  	void _auto_push_front(const value_type &val);
  	void _auto_expand();
  	template <class X> friend class blockList;
  };
  ```

  </font>

  * 构造函数

    初始化block时，将start和finish置于**存储区间的中间**，在前插入和后插入的情况下保证效率和空间利用率


  * <font color = LightSeaGreen face = consolas>typedef value_type* iterator</font>

    block\<T\>的迭代器block\<T\>::iterator

  * <font color = LightSeaGreen face = consolas>blockSize, blockData, storageStart, storageFinish, start, finish</font>

    * <font color = violet face = consolas>int</font> blockSize：block块的大小
    * <font color = violet face = consolas>iterator </font> blockData:  block块的存储结构
    * <font color = violet face = consolas>iterator </font> storageStart, storageFinish:  block块可用存储区间的首位位置
    * <font color = violet face = consolas>iterator </font> start, finish:  block块已存储空间的首位位置

  * <font color = violet face = consolas>blockSignal::signal</font> insert(*parameter*)

    insert函数实现了在block内随机访问插入的功能，能够实现单点插入和区间插入，并返回插入结果。实现的原理是判断插入点*position*相对于*start*和*finish*的位置，选择一种**能够移动尽可能少的数据的方案**，并尝试插入，返回插入结果(blockSignal)

  * <font color = violet face = consolas>blockSignal::signal</font> erase(*parameter*)

    erase函数实现了在block内随机访问删除的功能，能够实现单点删除和区间删除，并返回删除结果。实现的原理是判断删除点*position*相对于*start*和*finish*的位置，选择一种**能够移动尽可能少的数据的方案**，并尝试删除，返回删除结果(blockSignal)

  * <font color = violet face = consolas>void</font> clear()

    删除block内所有数据，恢复大小，重定位指针位置

  * <font color = violet face = consolas>void</font> \_auto_...(*parameter*)

    block的私有成员，在blockList中使用，由于blockList的实现是**用block来存储block\<T\>的指针**，该数据结构的插入用这些函数实现，每当区间插入失败时自动将区间长度扩大至原来的<font color = purple face = consolas>SINGLE_BLOCK_EXPANSION_RATE</font>倍（默认为2）

### blockList.h

* class blockList

  blockList基于block实现，通过将多个block串接在一起组成的数据结构（用block\<block\<T\> *\>实现）<font face = consolas size = 2>

  ```c++
  template <class T>
  class blockList {
  public:
  	typedef T value_type;
  	typedef typename block<block<T> *>::iterator block_table_itr;
  	typedef typename block<T>::iterator block_itr;	
  	class iterator {...};
  	typedef const iterator const_iterator;
  private:
  	int blockDefaultSize, blockCurrentSize;
  	block<block<T> *> blockTable;
  	iterator start, finish;
  public:
  	iterator begin() { return start; }
  	iterator end() { return finish; }
  	value_type& front() { return *start; }
  	value_type& back() { return *(finish - 1); }
  	const_iterator begin() const { return start; }
  	const_iterator end() const { return end; }
  	int size() const { return (finish - start); }
  	blockList();
  	blockList(int);
  	blockList(int, value_type*, value_type*);
  	blockList(const blockList<T> &);
  	void insert(iterator, const value_type &);
  	void insert(iterator, iterator, iterator);
  	void insert(iterator, value_type*, value_type*);
  	void erase(iterator);
  	void erase(iterator, iterator);
  	void push_back(const value_type &); //基于insert
  	void push_front(const value_type &); //基于insert
  	void pop_back(); //基于erase
  	void pop_front(); //基于erase
  	void clear();
  	void compress(int _newDefaultSize = 0);
  	bool empty() { return (start == finish); }
  	blockList<T>& operator=(const blockList<T> &);
  	value_type& operator[](int i) { return *(start + i); }
  private:
  	void _refresh_iterator();
  	friend class iterator;
  	friend class block<T>;
  };
  ```

  </font>

  * 构造函数

    提供了默认构造函数，赋值构造函数，可改变默认block大小的构造函数，通过blockList的iterator初始化的构造函数，通过value_type*进行初始化的构造函数

  * <font color = violet face = consolas>class</font> iterator

    iterator为blockList的嵌套类，符合随机迭代器的要求<font face = consolas size = 2>

    ```c++
    class iterator {
    public:
    		typedef std::random_access_iterator_tag iterator_category;
    		typedef T value_type;
    		typedef value_type* pointer;
    		typedef value_type& reference;
    		typedef std::ptrdiff_t difference_type; //STL对于随机迭代器的要求声明

    private:
    		class blockList<T> *iBlockList;
    		block_table_itr blockTableItr;
    		block_itr blockItr;
    public:
    		operator...
            ... //重载了各种运算符
            friend class blockList;
    };
    ```

    </font>

    * <font color = violet face = consolas>class blockList\<T\></font> *iBlockList

      指向所在blockList的指针，便于操作

    * <font color = violet face = consolas>block_table_itr</font> blockTableItr

      该迭代器代表数据所在block在blockList中的迭代器

    * <font color = violet face = consolas>block_itr</font> blockItr

      该迭代器代表数据所在block的迭代器

  * <font color = violet face = consolas>void</font> insert(*parameter*)

    在blockList中实现了随机单点插入，随机区间插入（可以用迭代器或指针）

    **单点插入时如果区间满将移动区间的一半到新建的block中（尽量减少操作）**

    **区间插入时将优先填满所插入的区间和后（前）一个区间，然后再新建block（增加空间利用率）**

  * <font color = violet face = consolas>void</font> erase(*parameter*)

    通过迭代器实现单点删除和区间删除，若有block中元素被全班删除，则将该block从block\<block *\>中删除

  * <font color = violet face = consolas>void</font> clear()

    删除blockList内所有数据，恢复大小，重定位指针位置

  * <font color = violet face = consolas>void</font> compress(<font color = violet face = consolas>int 

    </font> _newDefaultSize = 0)

    实现压缩功能，无参数时将blockLis中的block压缩为默认大小，有参数时压缩为_newDefaultSize大小

  * <font color = violet face = consolas>void</font> _refresh_iterator()

    由于**任何对blockList的修改删除操作都会使迭代器无效**，该函数在所有的修改删除函数后维护blockList内置的start和finish迭代器