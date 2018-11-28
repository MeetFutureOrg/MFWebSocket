//
//  MFWebSocketManager.h
//  MFWebSocket
//
//  Created MF XieBangyao on 2018/11/28.
//  Copyright © 2018 meetfuture. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SRWebSocket.h>

#define MFWebSocketShareManager [MFWebSocketManager shareManager]

extern NSString * const kSocketConnectSuccessNotification;
extern NSString * const kSocketConnectFailNotification;
extern NSString * const kSocketCloseNotification;
extern NSString * const kSocketReconnectingNotification;
extern NSString * const kSocketReconnectFailNotification;
extern NSString * const kSocketReceiveMessageNotification;

typedef NSString *MFSocketClosedInfo;

FOUNDATION_EXPORT MFSocketClosedInfo const MFSocketClosedCodeKey;
FOUNDATION_EXPORT MFSocketClosedInfo const MFSocketClosedReasonKey;
FOUNDATION_EXPORT MFSocketClosedInfo const MFSocketClosedCleanFlagKey;

@protocol MFWebSocketManagerDelegate <NSObject>

- (void)MFWebSocket:(SRWebSocket *)webSocket didReceiveMessage:(NSDictionary *)message;

@optional

- (void)MFWebSocketDidOpen:(SRWebSocket *)webSocket;
- (void)MFWebSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error;
- (void)MFWebSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean;
- (void)MFWebSocket:(SRWebSocket *)webSocket didReceivePong:(NSData *)pongPayload;

- (BOOL)MFWebSocketShouldConvertTextFrameToString:(SRWebSocket *)webSocket;

@end

typedef NS_ENUM(NSInteger, MFSocketReceiveType){
    MFSocketReceiveTypeForMessage,
    MFSocketReceiveTypeForPong
};

typedef NS_ENUM(NSInteger, MFSocketCode){
    MFSocketTimeoutCode                    = 504,  //正常定义，符合http 504超时定义
    MFSocketCloseNormalCode                = 1101, //自定义，socket正常关闭
    MFSocketCloseErrorCode                 , //自定义，socket异常关闭
    MFSocketCloseToReconnectCode           , //自定义，socket重连之前关闭
    MFSocketCloseReconnectFailCode         , //自定义，socket重连失败关闭
    MFSocketEmptyCode                      , //自定义，socket被置空
    MFSocketChangedCode                    , //自定义，存在两个以上socket实例（这种情况发生在重连逻辑，[socket close]和socket=nil未等待didclose代理回调完成，就调用新的[socket open]方法）
    MFSocketReceiveWrongFormatDataCode     , //自定义，服务端返回数据没有返回唯一识别ID
    MFSocketReceiveWrongIdentifyIdCode     , //自定义，服务端返回数据的唯一识别ID有误
    
};

typedef void(^MFSocketDidConnectBlock)(void);

typedef void(^MFSocketDidFailBlock)(NSError *error);

typedef void(^MFSocketDidCloseBlock)(NSInteger code,NSString *reason,BOOL wasClean);

typedef void(^MFSocketDidReceiveBlock)(NSDictionary *message, MFSocketReceiveType type);

@interface MFWebSocketManager : NSObject

@property (nonatomic, strong, readonly) SRWebSocket *webSocket;
@property (nonatomic, weak) id<MFWebSocketManagerDelegate> delegate;

@property (nonatomic, copy, readonly) MFSocketDidConnectBlock connectSuccessBlock;
@property (nonatomic, copy, readonly) MFSocketDidFailBlock connectFailBlock;
@property (nonatomic, copy, readonly) MFSocketDidCloseBlock closeBlock;
@property (nonatomic, assign, readonly) SRReadyState socketReadyState;

/**
 * 发送心跳 和后台约定发送什么内容
 * 默认 pingInfo = @"ping"，pingInfo类型需要时NSString或者NSData类型，不能为nil，SocketRocket可以为nil
 * 注意：只做了NSString的类型转换，不为NSString类型时，需要自己转成NSData类型
 */
@property (nonatomic, strong) id pingInfo;

NS_ASSUME_NONNULL_BEGIN

/**
 * 手动开启心跳，默认为NO，在didOpen回调中自动启动心跳
 */
@property (nonatomic, assign) BOOL manualStartHeartBeat;

/**
 * 延迟多少秒启动heartBeat，默认为0
 */
@property (nonatomic, assign) NSTimeInterval startHeartBeatDelay;

/**
 * 重连次数，默认5次
 */
@property (nonatomic, assign) NSUInteger maxReconnectTimes;

/**
 * 超时时间
 * 默认是5秒
 * 如需更改，必须在"mf_open:connect:failure:"方法调用之前设置，否则该方法超时不生效，其他send请求超时可生效
 * 该属性对"mf_openWithRequest:connect:failure"无效
 */
@property (nonatomic, assign) NSTimeInterval timeoutInterval;

/**
 * ping不通的最大次数，默认3次，3次ping不通（6秒），则认为掉线
 */
@property (nonatomic, assign) NSUInteger maxPingTimes;

/**
 * ping发送间隔，默认2秒一次
 */
@property (nonatomic, assign) NSTimeInterval pingInterval;

/**
 * 错误域名，默认：com.xby.XBYSocket
 */
@property (nonatomic, strong) NSString *errorDomain;

/**
 * 第一次open socket失败是否需要启动重连，默认NO
 */
@property (nonatomic, assign) BOOL openFailNeedReconnect;

+ (instancetype)shareManager;

/**
 * 打开socket
 *
 * @param urlStr urlString
 * @param connectBlock 打开连接成功回调
 * @param failureBlock 打开连接失败回调
 */
- (void)mf_openWithUrlString:(NSString *)urlStr connect:(MFSocketDidConnectBlock)connectBlock failure:(MFSocketDidFailBlock)failureBlock;

/**
 * 打开socket
 *
 * @param url url
 * @param connectBlock 打开连接成功回调
 * @param failureBlock 打开连接失败回调
 */
- (void)mf_openWithUrl:(NSURL *)url connect:(MFSocketDidConnectBlock)connectBlock failure:(MFSocketDidFailBlock)failureBlock;

/**
 * 打开socket
 * timeoutInterval对这个方法不起作用，因为timeoutInterval是加在request的参数里面的
 * @param request requset可以包裹Url/CachePolicy/TimeoutInterval等信息
 * @param connectBlock 打开连接成功回调
 * @param failureBlock 打开连接失败回调
 */
- (void)mf_openWithRequest:(NSURLRequest *)request connect:(MFSocketDidConnectBlock)connectBlock failure:(MFSocketDidFailBlock)failureBlock;

/**
 * 发送消息
 *
 * @param dicData 消息Dictionary
 * @param receiveBlock 收到服务端数据返回回调
 * @param failureBlock 未收到服务端数据返回回调
 */
- (void)mf_send:(NSDictionary *)dicData receive:(MFSocketDidReceiveBlock)receiveBlock failure:(MFSocketDidFailBlock)failureBlock;

/**
 * 手动启动HeartBeat，调用之后会将manualStartHeartBeat置为YES，再将manualStartHeartBeat更改回NO之前，都要自己手动启动
 *
 * @param delay 延迟
 */
- (void)mf_manualStartHeartBeatAfterDelay:(NSTimeInterval)delay;

/**
 * 主动关闭socket
 *
 * @param closeBlock 关闭回调
 */
- (void)mf_closeSocketWithBlock:(MFSocketDidCloseBlock)closeBlock;

NS_ASSUME_NONNULL_END

@end



