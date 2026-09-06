# Osten

Osten 是一个固定规模、非 Transformer、非 next-token 的本地决策引擎。

它的定位是**引擎基础层**，负责把输入表征、当前状态、目标、记忆上下文和心跳信号送入固定规模的决策网络，并输出状态变化与动作结果。Osten 本身不是完整的数字模型，也不负责承担全部训练、环境模拟或长期学习能力。

## 项目分层

```text
Astrax       数字模型与训练系统
  ├─ 状态预测
  ├─ 动作价值
  ├─ 好奇心 / 内在奖励
  ├─ Offline RL
  └─ 其他外部训练与环境适配

Osten        本地决策引擎基础层
  ├─ 输入编码
  ├─ 固定规模前向网络
  ├─ 状态与目标更新
  ├─ 动作指令输出
  ├─ 记忆上下文
  └─ 心跳驱动的自主运行
```

Astrax 计划在数字模型中扩展真正的状态预测、动作价值、内在奖励和 Offline RL 训练模块；Osten 则作为其中的决策引擎基础。两者是分层关系，不代表 Osten 当前已经实现这些完整训练能力。

## 设计约束

Osten 遵循 `todolist.md` 中的硬性约束：

- 核心决策不能由关键词、规则命中或检索结果直接产生。
- 文本必须先经过编码器转换为固定维度向量，原始字符串不能直接进入网络。
- 规则只能用于边界保护，不能参与核心决策链。
- 记忆只提供上下文，不直接选择答案或动作。
- 不使用 Transformer、self-attention 或 next-token 作为核心实现。
- 核心输出由固定规模神经网络前向生成。
- 无文本输入时，也必须能够通过心跳信号持续运行并产生状态变化。
- 模型规模在配置时确定，运行过程中不动态扩容。

## 当前实现

当前仓库使用 **C++17 + MSVC 原生 Visual Studio 工程**，不使用 CMake。

已实现的基础能力：

- 固定字符 n-gram 哈希编码器：文本先转为状态向量。
- 固定规模前向策略网络：输出状态变化、动作指令、记忆写入、内部计划变化和置信度。
- 有界情景记忆：只提供上下文，不检索答案、不直接做决策。
- 心跳驱动的自主 tick：无文本输入时仍持续产生状态变化。
- 数值有限值/范围保护：仅用于边界安全，不参与核心推理。
- 基础训练接口和评测接口骨架。
- 不包含 self-attention、词表、next-token 解码或文本生成核心链路。

当前版本的重点是验证引擎能够稳定运行，而不是证明训练效果或数字模型能力。

## 与 Astrax 的关系

Astrax 是承载数字模型、训练流程和外部环境的上层项目。Osten 可以向 Astrax 提供：

- 固定格式的状态、目标和动作接口。
- 可重复的本地前向决策过程。
- 记忆上下文和动作结果。
- 心跳驱动的连续运行能力。
- 用于训练和评测的基础数据结构。

Astrax 可以在 Osten 之上接入状态转移数据、动作价值估计、内在奖励、策略优化以及 Offline RL。具体训练算法属于 Astrax 层，不属于当前 Osten 基础引擎的职责范围。

项目地址：[`Qxyz17/Astrax`](https://github.com/Qxyz17/Astrax)

## MSVC 构建

使用 Visual Studio 2022 的 **Developer PowerShell**：

```powershell
.\build-msvc.ps1
```

或直接打开 `Osten.sln`，选择 `Release | x64` 后生成。输出：

```text
build\Release\osten.lib
build\Release\osten_demo.exe
build\Release\osten_tests.exe
```

运行：

```powershell
.\build\Release\osten_demo.exe 8
.\build\Release\osten_demo.exe 8 "observe local environment"
.\build\Release\osten_tests.exe
```

## 目录

```text
include\osten\   公共头文件
src\             引擎、编码器、记忆、网络、训练和评测实现
tests\           原生 C++ 测试
docs\            架构约束说明
Osten.sln        Visual Studio 解决方案
*.vcxproj        MSVC 项目文件
```

## 当前阶段

版本：`0.1.4-alpha`  
日期：`2026-09-06`

当前阶段目标是完成稳定、可扩展的决策引擎基础。状态预测、动作价值、内在奖励和 Offline RL 等完整训练模块将在 Astrax 的数字模型中继续实现。
