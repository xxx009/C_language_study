# C_language_study
辅助教学

## 地铁换乘规划示例

新增了一个地铁换乘规划示例，支持：

- 地铁线路维护（新增、删除、线路站点关系维护）
- 地铁站点维护（新增、删除）
- 换乘站点独立维护（新增/删除换乘点，并设置换乘连接及时间）
- 列车各站点运行时间数据维护
- 预留线路选择优化接口（可注入自定义代价函数）
- 地图展示（按行列网格打印线路站点）
- 输入起点与终点，输出最优路线与耗时
- 可扩展运力与运营策略：线路配车、分时段发车间隔（如 30 分钟/班、3 分钟/班）
- 可扩展客流建模：支持站点配置均匀分布、泊松分布、t 分布参数
- 预算与成本控制：支持运营成本参数配置和预算估算
- 支持 JSON 导入/导出基础数据（`subway_data.json`）
- 提供命令行 UI 维护入口（新增站点/线路、导出）
- 收益估算按每相邻1站每人0.1元，运营成本按每相邻1站每人0.4元

### 编译与运行

```bash
gcc -Iinclude src/subway/main.c src/subway/subway_core.c src/subway/subway_operations.c src/subway/subway_transfer_budget.c src/subway/subway_routing.c -o subway_planner
./subway_planner
# 按提示输入：起点 终点 乘客数（如：0 5 20）
```
