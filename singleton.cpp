//单例的几种实现方式
//1.内部静态变量实现--懒汉模式
//构造，析构，拷贝构造，赋值构造全部设置为私有，禁止外部的构造与析构
class Single
{
  public:
	  static Single &GetInstance();//静态函数，返回一个静态类，返回类型为引用,如果//返回指针，则可能被调用delete删除。
	  void Print();//打印地址验证是否只产生了一个实例对象。
  private:
          Single();
	  ~Single();
	  Single(const Single &signal);
          const Single &operator=(const Single &single);
};

 Single & Single::GetInstance()
  {
	  static Single single;
	  return single;
  }

  void Print()
  {
	  std::cout<<"本实例内存地址为："<<this<<std::endl;
  }
Single::Single()
{
  std::cout<<"调用了构造函数"<<endl;
}
Single::~Single()
{
   std::cout<<"调用了析构函数"<<endl;
}

//对于这种方法，其代码量少，不需要加锁就实现了线程安全（c++11标准下），也即Magic Static特性，经典方法。

//2.普通懒汉模式实现 --线程不安全
 class SingleInstance
{
  public: 
	  static SingleInstance *GetInstance();//静态函数，返回值为一个指针；
          static void deleteInstance();//释放单例的指针；
	 void Print();//打印实例地址
 private:
	 SingleInstance();
	 ~SingleInstance();
	 SingleInstance(const SingleInstance&singal);
	 const SingleInstance &operator=(const SingleInstance &singal);
	 static SingleInstance *m_singal;
}

SingleInstance*SingleInstance::m_singal=nullptr;

SingleInstance*SingleInstance::GetInstance()
{
   if(m_singal==nullptr)
	   m_singal=new(std::nothrow)SingleInstance;
   return m_singal;
}

void SingleInstance::deleteInstance()
{
  if(m_singal)
  {
    delete m_singal;
    m_singal=nullptr;
  }
}

void SingleInstance::Print()
{
  std::cout<<"本实例内存地址为："<<std::endl;
}
SingleInstance::SingleInstance()
{
  std::cout<<"调用了构造函数"<<std::endl;
}
SingleInstance::~SingleInstance()
{
   std::cout<<"调用了析构函数"<<std::endl;
}

//3.普通懒汉模式实现 --加锁，DCL实现  其线程安全
 class SingleInstance
{
  public:
          static SingleInstance *&GetInstance();//静态函数，返回值为一个指针；
          static void deleteInstance();//释放单例的指针；
         void Print();//打印实例地址
 private:
         SingleInstance();
         ~SingleInstance();
         SingleInstance(const SingleInstance&singal);
         const SingleInstance &operator=(const SingleInstance &singal);
         static SingleInstance *m_singal;
	 static std::mutex m_mutex;
}

SingleInstance*SingleInstance::m_singal=nullptr;
std::mutex SingleInstance::m_mutex;
SingleInstance*SingleInstance::GetInstance()
{
   if(m_singal==nullptr)
   { 
	   std::unique_lock<std::mutex>lock(m_mutex);
	   if(m_singal==nullptr)
           m_singal=new(std::nothrow)SingleInstance;
   }
   return m_singal;
}

void SingleInstance::deleteInstance()
{
	std::unique_lock<std::mutex> lock(m_mutex);//无需检查，因为delete调用之后会将指针指向nullptr，即使多次delete也没问题。
  if(m_singal)
  {
    delete m_singal;
    m_singal=nullptr;
  }
}

void SingleInstance::Print()
{
  std::cout<<"本实例内存地址为："<<std::endl;
}
SingleInstance::SingleInstance()
{
  std::cout<<"调用了构造函数"<<std::endl;
}
SingleInstance::~SingleInstance()
{
   std::cout<<"调用了析构函数"<<std::endl;
}
//4.饿汉模式  --直接先生成一个实例，不需要在调用时才生成，缺点是会占用内存，即使不适用该类或者实例，尤其当类比较大时。
class SingleInstance
{

public:
    // 获取单实例对象
    static SingleInstance *GetInstance();

    //释放单实例，进程退出时调用
    static void deleteInstance();
	
    // 打印实例地址
    void Print();

private:
    // 将其构造和析构成为私有的, 禁止外部构造和析构
    SingleInstance();
    ~SingleInstance();

    // 将其拷贝构造和赋值构造成为私有函数, 禁止外部拷贝和赋值
    SingleInstance(const SingleInstance &signal);
    const SingleInstance &operator=(const SingleInstance &signal);

private:
    // 唯一单实例对象指针
    static SingleInstance *m_SingleInstance;
};

//初始化静态成员变量
SingleInstance *SingleInstance::m_SingleInstance = new(std::nothrow)SingleInstance;

SingleInstance *&SingleInstance::GetInstance()
{
    return m_SingleInstance;
}

void SingleInstance::deleteInstance()
{
    if (m_SingleInstance)
    {
        delete m_SingleInstance;
        m_SingleInstance = NULL;
    }
}

void SingleInstance::Print()
{
	std::cout << "我的实例内存地址是:" << this << std::endl;
}

SingleInstance::SingleInstance()
{
    std::cout << "构造函数" << std::endl;
}

SingleInstance::~SingleInstance()
{
    std::cout << "析构函数" << std::endl
}
//5.单例模板：
  template<typename T>
  class Singleton{
   public:
	   static T&get_instance()
	   {
              static T instance;
	      return T;
	   }
	   virtual ~Singleton(){
		   std::cout<<"基类析构函数调用"<<std::endl;}
  private:
	   Singleton(const Singleton&);
	   Singleton &operator=(const Singleton&);
  protected:
	   Singleton(){
             std::cout<<"基类构造函数调用"<<std::endl;
	   }

  }

//举例：
class example:public Singleton<example>{
 friend class Singleton<example>;//友元以便于访问子类私有构造
  public：
	  example(const example&)=delete;
    example &operator=(const example&)=delete;
    private：
	    example()=default;
}

//具体可使用如下：
  example & t=example::get_instance();//继承了父类方法，并且已经实例化了模板，直接调用
