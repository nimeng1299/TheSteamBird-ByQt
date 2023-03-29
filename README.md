# TheSteamBird
一款原神数据查看工具，支持国服和国际服，可自定义添加插件



## 功能

体力查看，探索度查询，抽卡分析，深渊查询，以及其他作者编写的插件...

![overview](/mdsrc/overview.png)



## 加载插件

1. 下载插件文件
2. 放到软件目录下的  `/plugins/` 文件夹
3. 打开软件，即可看到插件已被加载



## 编写插件

1. 下载 `plugin demo` 插件代码示例

2. 修改 `description.json` 文件

   ```json
   {
   	"name": "pluginA",				//插件的名字
   	"nickname": "插件显示的名称",	   //插件在界面显示的名称
   	"author": "author",				//插件的作者
   	"version": "1.0.0",				//插件版本
   	"platform": 2,					//支持的平台，0、米游社 1、HoyoLab 2、全部支持
   	"description": "插件的描述"		 //插件的描述
   }
   ```

3. 修改 .pro 文件

   `TARGET = PluginName`

4. 在  `.h/.cpp/.ui` 文件中实现内容

5. 发布插件，选择 `realease` 模式，找到生成的 `.dll` 文件

6. 按照加载插件的方法加载插件

7. 若插件无法加载，可能是缺少必要的 `.dll` 文件

