# MFWebSocket
Encapsulate a WebSocket library base on Facebook's SocketRocket library.

封装Facebook的SocketRocket，实现每个send消息成功失败回调，连接超时，send超时。

***

# Notice 须知
**WebSocket与Htpp请求不同，默认是长连接，除open socket超时以外，SocketRocet没有为send命令实现超时。**
**若要实现send超时功能，则需与后台约定好每个send命令都要包含一个命令标识，并且每条命令服务端都需要应答。**
**这里的实现是在每一个send命令字典中添加了一个key叫identificationID的key-value（App端已自动添加，不需要自己额外添加），服务端只需要每次把这个identificationID原封不动返回即可。**

**用户命令参数：@{@"cmd": @"test"}**
**实际Send命令参数：@{@"cmd": @"test",@"identificationID": @"2018-11-30 15:33:06.34337576"}。**
**identificationID字段无需自己添加，但是需要服务端返回**
# Requirements 要求
iOS 7+
Xcode 8+

***

# Installation 安装
### 1.手动安装:
下载DEMO后,将子文件夹MFWebSocket拖入到项目中, 导入头文件MFWebSocket.h开始使用, 注意: 项目中需要有'SocketRocket', '~> 0.5.1'第三方库!

### 2.CocoaPods安装:
first pod 'MFWebSocket',:git => 'https://github.com/MeetFutureOrg/MFWebSocket.git' then pod install或pod install --no-repo-update

如果发现pod search MFWebSocket 不是最新版本，在终端执行pod setup命令更新本地spec镜像缓存(时间可能有点长),重新搜索就OK了

***

# Usage 使用方法
### 1. Open Socket 开启Socket
#### 1.1 使用NSString初始化
```oc
[MFWebSocketShareManager mf_openWithUrlString:@"http://192.168.0.1" connect:^{
        
} failure:^(NSError *error) {
        
}];
```

#### 1.2 使用NSURL初始化
```oc
[MFWebSocketShareManager mf_openWithUrl:[NSURL URLWithString:@"http://192.168.0.1"] connect:^{
        
} failure:^(NSError *error) {
        
}];
```
#### 1.3 使用NSURLRequest初始化
```oc
[MFWebSocketShareManager mf_openWithRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://192.168.0.1"] cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:5] connect:^{
        
 } failure:^(NSError *error) {
        
 }];
```

### 2. Send Message 发送消息
```oc
[MFWebSocketShareManager mf_send:@{@"cmd":@"read"} receive:^(NSDictionary *message, MFSocketReceiveType type) {
    
} failure:^(NSError *error) {
        
}];

```

### 3. Close Socket 关闭Socket
```oc
//主动关闭Socket
[MFWebSocketShareManager mf_closeSocketWithBlock:^(NSInteger code, NSString *reason, BOOL wasClean) {
        
}];
```

### 4. Start HeartBeat 开启HeartBeat
#### 4.1 自动开启
默认自动开启
#### 4.2 手动开启
```oc
MFWebSocketShareManager.manualStartHeartBeat = YES;//先设置需要手动开启

//手动开启
[MFWebSocketShareManager mf_manualStartHeartBeatAfterDelay:5.0];

```

### 5. Other Setting 其他设置
**这些设置都需要在open socket之前设置才会生效**

#### 5.1 ping info ping信息
pingInfo类型需要是NSSting或者NSData类型，内容与服务端定义
默认为@"ping"
```oc
@property (nonatomic, strong) id pingInfo;
```
#### 5.2 延迟多少秒自动开启HeartBeat
需要manualStartHeartBeat属性为NO才能生效
默认为0秒，不延迟启动
```oc
@property (nonatomic, assign) NSTimeInterval autoStartHeartBeatDelay;
```
#### 5.3 最大重连次数
最大重连次数
默认为5次
```oc
@property (nonatomic, assign) NSUInteger maxReconnectTimes;
```
#### 5.4 open与send超时时间
open与send超时时间
默认为5秒
```oc
@property (nonatomic, assign) NSTimeInterval timeoutInterval;
```
#### 5.5 最大ping次数
最大ping次数，多少次ping不通，则认为掉线
默认为3次
```oc
@property (nonatomic, assign) NSUInteger maxPingTimes;
```
#### 5.6 ping时间间隔
ping时间间隔，多少秒ping一次
默认为2秒
```oc
@property (nonatomic, assign) NSTimeInterval pingInterval;
```
#### 5.7 错误域
错误域，用于区别SocketRocket错误还是MFWebSocket定义的错误
默认为@"com.xby.XBYSocket" 
```oc
@property (nonatomic, strong) NSString *errorDomain;
```

# 联系方式:
Email : xiebangyao_1994@163.com
Blog : https://adrenine.github.io/

# 许可证
MFWebSocket 使用 MIT 许可证，详情见 LICENSE 文件。