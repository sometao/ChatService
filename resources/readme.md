### ChatService is a project for learning c++.


#### 主要功能
* 客户端连接服务器
* 服务器鉴权
* 客户端之间文本通信


#### 主要模块
* UserAgent  
  * 负责处理用户侧业务逻辑
  * 处理用户输入
  * 处理用户侧输出
  * 管理用户状态
* ChatClient
  * 持有SocketClient
  * 与ChatServer通信
  * 进行登陆
  * 消息编码与发送
  * 接受消息与解码，上报到UserAgent
* SocketClient
  * 通过socket API与SocketServer通信
  * 发送TCP消息
  * 接收TCP消息并上报到ChatClient
* ChatServer
  * 持有SocketServer，处理与ChatClient的消息通信  
  * 处理用户鉴权
  * 为用户建立连接
  * 转发用户间消息
* SocketServer
  * 通过socket API与SocketClient进行通信
  * 维护多个SocketClient
  * 采用select与多线程机制处理消息


#### 