# SPK32AE103NTO 配置框架（configs/）

项目的构建配置按**三部分**分层，物理隔离、可任意组合。所有配置经 `build.sh`
的参数组合成 `EXTRA_CONF_FILE` 传入 `west build`。

## 三部分定位

| 文件 | 定位 | 内容 | 客户可见 |
|------|------|------|----------|
| `base.conf` | ① 正式代码 | 底层必须、架构强依赖（如 CODE_DATA_RELOCATION） | ✅ 随交付 |
| `optional.conf` | ② 可选功能 | 可扩展、按需开关（如 INIT_STACKS、WDOG_INIT、MP_MAX_NUM_CPUS） | ✅ 可选项 |
| `test.conf` | ③ 测试代码 | 内部测试专用（如 COVERAGE_GCOV） | ❌ 交付时排除 |
| `opt.conf` | ④ 编译选项 | 优化等级/-g/-flto/额外 RISC-V flag，对齐 common.mk | ✅ 随交付 |

> `opt.conf` 与前三者不同：它**不属于功能分层**，而是「编译参数」。`build.sh`
> 会**固定引入**（不受 `PARTS` 影响），改它就能切换优化等级、调试信息、LTO 等，
> 无需改 `build.sh` 或命令行。默认 `-O2`（Zephyr 原生最高档），要试 `-O3`/`-Ofast`
> 见 `opt.conf` 内注释。

## 组合用法

```bash
./build.sh                            # 缺省：内部测试 = base + optional + test
./build.sh base optional              # 正式交付（显式排除 test）
./build.sh base test                  # 仅 base + test
./build.sh base optional test         # 全量（内部测试，等价缺省）
./build.sh test                       # 仅 test（极端调试）
```

任意子集组合都有效；未知部分名会报错退出。

`test` 是**软性部分**：`build.sh` 会自动检查 `test.conf` 是否存在，不存在则
跳过并提示。因此交付给客户时直接删掉 `test.conf` 即可，无需改动 `build.sh`。

## 与上游 conf 的关系

- `prj.conf`：上游 EC 继承主配置，**始终加载**，本框架不动它。
- `ae103_nto_minimal.conf`：AE103 最小验证覆盖（暂无 systimer，`CONFIG_LOG=n`），
  `build.sh` 会固定追加，待 systimer 落地后移除。
- `debug.conf` / `release.conf` / `safdebug.conf`：上游遗留，当前未接入本框架。

## CODE_DATA_RELOCATION 前置条件（重要）

`CONFIG_CODE_DATA_RELOCATION=y` 是「能力开关」，不是「开完即用」：

1. 开启后，启动代码 `kernel/xip.c` / `kernel/init.c` 会调用
   `data_copy_xip_relocation()` / `bss_zeroing_relocation()`；
2. 这两个符号由 CMake 的 `zephyr_code_relocate()` 生成；
3. 因此开启开关的同时，必须在 `CMakeLists.txt` 里用 `zephyr_code_relocate()`
   指定待搬移的驱动文件，否则链接报 undefined reference。

当前 SPI Flash 驱动尚未落地，故 `base.conf` 里该项为**注释预留**。待驱动就绪后：
去掉注释 + 在 CMakeLists.txt 补 `zephyr_code_relocate()` 调用。
