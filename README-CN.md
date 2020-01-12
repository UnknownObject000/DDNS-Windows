[English](./README.md) | 简体中文

# DDNS-linux 
一个C++编写的DDNS程序，可实现**阿里云**域名的动态解析记录修改，同时支持IPv4和IPv6。这是Windows版本。**警告：WindowsXP/Windows Server 2003及以下版本系统不受支持。**

## 编译
编译此程序需要以下程序/组件：
  - Visual Studio 2017或以上版本 (支持c++11)
  - <a href="https://github.com/aliyun/aliyun-openapi-cpp-sdk">Aliyun-openapi-cpp-sdk</a>的 ***core*** 和 ***alidns*** 组件
  - 安装了libcurl
  - 安装了SrvanyUI

编译步骤：
  1. 执行以下命令将源代码下载到设备上。
  ```bash
    git clone https://github.com/UnknownObject000/DDNS-linux.git
  ```
  
  2. 安装SrvanyUI
  
      运行3rd文件夹中的install.exe安装。（需要管理员权限）
  2. 编译并安装。
      - 打开Visual Studio
      - 打开项目文件
      - 选择合适的平台（32位系统选择x86，64位系统选择x64）
      - 点击 生成->生成DDNS_CPP 编译服务程序
      
  3. 编辑配置文件
  
   **config.uson**
    
   config.uson存储了Accesskey信息、dns.uson和日志文件的位置，此文件必须与程序放到同一目录下。 (默认目录是 C:/Windows/System32)
    
   参考源代码中的 'config.uson' 并填写自己的数据。(不包括'<' 和 '>')
    
   **dns.uson**
    
   dns.uson 记录了域名和DNS解析记录信息，它的目录需要填写在 'config.uson' 中。
   
   参考源代码中的 'dns.uson' 并填写自己的数据。(不包括'<' and '>').确保该文件下的所有域名属于同一账户。当你只有一个域名时， 'DOMAIN_COUNT 和所有的 '<no.>' 必须被设置为1.如果你有多个域名，序号为从 1 到 n.
   
   **log**
   
   日志文件的名称是任意的。你只需要确保存储日志文件的文件夹存在而不需要创建日志文件。程序会自动创建它。
   
  4. 安装服务
      - 打开SrvantUI
      - 点击➕，选择“系统服务”
      - 选择DDNS_CPP.exe作为服务程序
      - 输入服务名称和服务描述，保存。
  
  5. 复制dll文件
      将dll文件夹下的所有dll文件复制到程序所在目录。
 
 ## 使用方法
    服务安装完成后会自动启动。可以在系统的服务管理器中查看和管理服务。

