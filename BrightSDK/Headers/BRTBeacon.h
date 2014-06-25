//
//  BRTBeacon.h
//  BrightSDK
//
//  Version : 1.0.0
//  Created by Bright Beacon on 21/04/14.
//  Copyright (c) 2014 Bright. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import "BRTBeaconDefinitions.h"

#define DEFAULT_UUID @"E2C56DB5-DFFB-48D2-B060-D0F5A71096E0"
#define DEFAULT_MAJOR 0
#define DEFAULT_MINOR 0
#define DEFAULT_MEASURED -55
#define DEFAULT_LED 1
#define DEFAULT_INTERVAL 250
#define DEFAULT_TX 2
#define DEFAULT_NAME  @"BrightBeacon"

@class BRTBeacon;

////////////////////////////////////////////////////////////////////
// Bright Beacon delegate protocol


/**
 
 BRTBeaconDelegate 定义了 beacon 连接相关的委托方法，beacon 的连接是一个异步操作，因此你只需要实现例如beaconDidDisconnectWith:相关方法，它们会被自动回调.
 
 */

@protocol BRTBeaconDelegate <NSObject>

@optional

/**
 * beacon连接产生错误回调该方法.
 *
 * @param beacon 关联的beacon实体
 * @param error 错误的描述信息
 *
 * @return void
 */
- (void)beaconConnectionDidFail:(BRTBeacon*)beacon withError:(NSError*)error;

/**
 * beacon连接成功回调该方法
 *
 * @param beacon 关联的beacon实体
 *
 * @return void
 */
- (void)beaconConnectionDidSucceeded:(BRTBeacon*)beacon;

/**
 * beacon与设备已经断开连接回调该方法
 *
 * @param beacon 关联的beacon实体
 * @param error 错误的描述信息
 *
 * @return void
 */
- (void)beaconDidDisconnect:(BRTBeacon*)beacon withError:(NSError*)error;

@end


////////////////////////////////////////////////////////////////////
// Interface definition

/**
 
 BRTBeacon类代表了一个在该区域监听中遇到的beacon设备. 你不需要直接实例化这个类. BRTBeaconManager在扫描到beacon设备时会回调相应delegate的方法传人BRTBeacon.你可以通过BRTBeacon里的信息来识别不同的设备.
 
 
 BRTBeacon类既包含了苹果CLBeacon类的基本方法，同时也新增了一些扩展方法，它能连接到beacon设备读取和写人一些特征（characteristics）.
 
 */

@interface BRTBeacon : NSObject <CBPeripheralDelegate>

extern CBCentralManager *centralManager;

@property (nonatomic, weak)     id <BRTBeaconDelegate>  delegate;

/////////////////////////////////////////////////////
// 蓝牙设备也可以通过下面的方法识别到
// startBrightBeaconsDiscoveryForRegion:

/**
 *  invalidTime
 *
 *  Discussion:
 *    beacon设备上一次出现的时间戳，一般用于维护是设备生存周期，例如5s内未再次收到该设备信号，就移除该beacon设备.
 */
@property (nonatomic, unsafe_unretained) NSInteger invalidTime;

/**
 *  macAddress
 *
 *  Discussion:
 *    beacon设备的物理地址.建议使用该值作为Beacon标识
 */
@property (nonatomic, strong)   NSString*               macAddress;

/**
 *  name
 *
 *    beacon设备名.
 *
 */
@property (nonatomic, strong)   NSString*               name;

/**
 *  proximityUUID
 *
 *    beacon设备临近区域的UUID.
 *
 */
@property (nonatomic, strong)   NSUUID*                 proximityUUID;

/**
 *  major
 *
 *    区域关联的主要属性值. 如果未设定该值，默认是0.
 *
 */
@property (nonatomic, strong)   NSNumber*               major;

/**
 *  minor
 *
 *    区域关联的次要属性值. 如果未设定该值，默认是0.
 *
 */
@property (nonatomic, strong)   NSNumber*               minor;



/**
 *  rssi
 *
 *    beacon的接收信号强度指示（Received Signal Strength Indicator）以分贝为单位.
 *    该值是根据本次beacon发射的信号所收集到样本的平均值.
 *
 */
@property (nonatomic)           NSInteger               rssi;

/**
 *  distance
 *
 *    根据接收信号强度指示（rssi）和测量功率（measured power：距离一米的rssi值）计算出的beacon设备到手机的距离.
 *
 */
@property (nonatomic, strong)   NSNumber*               distance;

/**
 *  proximity
 *
 *    该值代表着一般意义上的相对距离远近，可以通过它快速确定beacon设备距离用户是在附近或是很远.
 */
@property (nonatomic)           CLProximity             proximity;

/**
 *  measuredPower
 *
 *    该值是1米处的rssi值，用于设备校准.
 */
@property (nonatomic, strong)   NSNumber*               measuredPower;

/**
 *  region
 *
 *    该值是设备所在region.
 */
@property (nonatomic, strong)   CLBeaconRegion*               region;

/**
 *  peripheral
 *
 *    代表一个周边设备，用于蓝牙连接.
 */
@property (nonatomic, strong)   CBPeripheral*           peripheral;

/////////////////////////////////////////////////////
// 通过蓝牙连接，读取的属性

/// @name 连接之后属性可用


/**
 *  isConnected
 *
 *    标示连接状态.
 */
@property (nonatomic, readonly)   BOOL                  isConnected;

/**
 *  power
 *
 *    以分贝计的发射功率，连接后可用
 */
@property (nonatomic, unsafe_unretained)   BRTBeaconPower           power;

/**
 *  advInterval
 *
 *    广播发射间隔，值范围100ms~10000ms,连接后可用
 */
@property (nonatomic, strong)   NSNumber*               advInterval;

/**
 *  ledState
 *
 *    led灯状态，连接后可用
 */
@property (nonatomic, unsafe_unretained)   BOOL          ledState;



/// @name 连接beacon相关的方法


/**
 * 蓝牙连接到beacon设备
 * 只能连接之后才可以修改
 * Major, Minor, Power and Advertising interval.
 *
 * @return void
 */
-(void)connectToBeacon;

/**
 * 断开蓝牙连接
 *
 * @return void
 */
-(void)disconnectBeacon;


/// @name 读取beacon配置信息相关的方法


/**
 * 读取连接中的beacon设备名 (要求已经连接成功)
 *
 * @param completion 读取设备名完成回调
 *
 * @return void
 */
- (void)readBeaconNameWithCompletion:(BRTStringCompletionBlock)completion;

/**
 * 读取连接中的beacon设备临近的UUID值 (要求已经连接成功)
 *
 * @param completion 读取ProximityUUID值回调
 *
 * @return void
 */
- (void)readBeaconProximityUUIDWithCompletion:(BRTStringCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的major值 (要求已经连接成功)
 *
 * @param completion 读取major值完成回调
 *
 * @return void
 */
- (void)readBeaconMajorWithCompletion:(BRTUnsignedShortCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的minor值 (要求已经连接成功)
 *
 * @param completion 读取minor值完成回调
 *
 * @return void
 */
- (void)readBeaconMinorWithCompletion:(BRTUnsignedShortCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的发射间隔 (要求已经连接成功)
 *
 * @param completion 读取发射间隔值完成回调
 *
 * @return void
 */
- (void)readBeaconAdvIntervalWithCompletion:(BRTUnsignedShortCompletionBlock)completion;


/**
 * 读取连接中的beacon设备的发射功率 (要求已经连接成功)
 *
 * @param completion 读取发射功率值完成回调
 *
 * @return beacon功率的float值
 */
- (void)readBeaconPowerWithCompletion:(BRTPowerCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的led灯状态 (要求已经连接成功)
 *
 * @param completion 读取led灯状态完成回调
 *
 * @return void
 */
- (void)readBeaconLedStateWithCompletion:(BRTBoolCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的测量功率 (要求已经连接成功)
 *
 * @param completion 读取测量功率值完成回调
 *
 * @return void
 */
- (void)readBeaconMeasuredPowerWithCompletion:(BRTShortCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的固件版本 (要求已经连接成功)
 *
 * @param completion 读取固件版本信息完成回调
 *
 * @return void
 */
- (void)readBeaconFirmwareVersionWithCompletion:(BRTStringCompletionBlock)completion;

/**
 * 读取连接中的beacon设备的硬件版本 (要求已经连接成功)
 *
 * @param completion 读取硬件版本信息完成回调
 *
 * @return void
 */
- (void)readBeaconHardwareVersionWithCompletion:(BRTStringCompletionBlock)completion;


/// @name 写人beacon配置信息相关的方法

/**
 * 写入设备名
 *
 * @param name 设备名
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconName:(NSString *)name withCompletion:(BRTStringCompletionBlock)completion;

/**
 * 写入 Proximity UUID 到连接中的蓝牙设备. 请注意虽然你是把UUID当作自己项目中私用，但是别人也可以读取、复制这个UUID来模仿你Beacon，所以如果你正开发一个很关键的应用程序的时候，安全是需要考虑的问题，你必须自己实现它。我们也尽力在保证我们beacon的安全性，在下一个版本中我们会提供更好的安全模式。
 *
 * @param pUUID 待写入的 Proximity UUID 值
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconProximityUUID:(NSString*)pUUID withCompletion:(BRTStringCompletionBlock)completion;

/**
 * 写入 major 到连接中的蓝牙设备.
 *
 * @param major 待写入的major值
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconMajor:(unsigned short)major withCompletion:(BRTUnsignedShortCompletionBlock)completion;

/**
 * 写入 minor 到连接中的蓝牙设备.
 *
 * @param minor 待写入的minor值
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconMinor:(unsigned short)minor withCompletion:(BRTUnsignedShortCompletionBlock)completion;

/**
 * 写入发射频率 (以毫秒计) 到连接中的蓝牙设备.
 *
 * @param advertising 待写入发送频率值 (100 - 10000 ms)
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconAdvInterval:(unsigned short)interval withCompletion:(BRTUnsignedShortCompletionBlock)completion;


/**
 * 写入发射功率到连接中的蓝牙设备
 *
 * @param power beacon设备发射功率值 (取值范围从 BRTBeaconPowerLevel1 /弱 到 BRTBeaconPowerLevel8 / 强)
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconPower:(BRTBeaconPower)power withCompletion:(BRTPowerCompletionBlock)completion;

/**
 * 写入led灯状态到连接中的蓝牙设备
 *
 * @param ladState led灯状态 （YES 打开 NO 关闭）
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconLedState:(BOOL)ledState withCompletion:(BRTBoolCompletionBlock)completion;

/**
 * 写入测量功率到连接中的蓝牙设备
 * @param MeasuredPower 测量功率 （1米处的rssi值）
 * @param completion 写入完成回调
 *
 * @return void
 */
- (void)writeBeaconMeasuredPower:(short)measurePower withCompletion:(BRTShortCompletionBlock)completion;

/// @name 固件更新相关方法

/**
 * 重置beacon设备默认值，该操作要求已经成功执行 [BRTBeaconManager registerApp:YOUR_KEY];
 *
 *
 * @return void
 */
-(void)resetBeaconToDefault;

/**
 * 重置beacon设备默认KEY，该操作可以解除设备开发者绑定，让beacon设备可以重新被连接设定新的YOUR_KEY，该操作要求已经成功执行[BRTBeaconManager registerApp:YOUR_KEY];
 *
 * @return void
 */
- (void)resetSDKKEY;

@end
