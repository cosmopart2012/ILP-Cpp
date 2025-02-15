# ILP-C++

## 关于此项目
此项目的前身：[ILP的Python版本](https://github.com/bkctwy/ILP)。


## 特性
- 使用协程
- 进度保存
- 缓存目录
- 保存为多章节文件
- 从Github安装插件
- 启用/禁用/卸载插件
- 通过插件实现多网站爬取

## TODO
- [ ] 进度显示
- [ ] 支持选择爬取范围
- [ ] 支持单文件输出
- [ ] 支持插件搜索
- [ ] 支持插件更新
- [x] ~~支持跨平台（当前未测试在Mac OS系统上是否可运行）~~

## 使用

### 运行（Windows and Linux）

#### Windows
1. 从[Github Actions](https://github.com/bkctwy/ILP-Cpp/actions)页面下载压缩包（Windows-{commit-id}）
2. 解压
3. 运行`ILP.exe --help`获取帮助

#### Linux
1. 从[Github Actions](https://github.com/bkctwy/ILP-Cpp/actions)页面下载压缩包（Linux-{commit-id}）
2. 解压
3. 运行`ILP --help`获取帮助

## 配置
```jsonc
{
    "data_folder": "data",     // 总数据目录
    "novels_folder": "novels", // 小说数据目录
    "logs_folder": "logs",     // 日志目录
    "covers_folder": "covers", // 封面图片目录
    "max_workers": "7",        // 最大协程数(最好不要大于10)
    "sleep_time": "5"          // 请求间隔(最好不要小于3)
}
```

## 注意事项
- 请不要滥用本程序，且用且珍惜
- 用户使用本程序造成的一切后果请自行承担
- 通过本程序爬取的小说需遵守相应网站的版权声明
- 本程序仅供学习交流使用，请勿用于商业用途
