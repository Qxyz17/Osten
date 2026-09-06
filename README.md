# Osten

Osten 是一个固定规模、非 Transformer、非 next-token 的本地决策引擎。

当前实现使用 **C++17 + MSVC 原生 Visual Studio 工程**，不使用 CMake。

## 已实现

- 固定字符 n-gram 哈希编码器：文本先转为状态向量，禁止原始字符串直接进入网络。
- 固定规模前向策略网络：输出状态变化、动作指令、记忆写入、内部计划变化和置信度。
- 有界情景记忆：只提供上下文，不检索答案、不直接做决策。
- 心跳驱动的自主 tick：无文本输入时仍持续产生状态变化。
- 数值有限值/范围保护：仅用于边界安全，不参与核心推理。
- 基础训练接口和评测接口骨架，便于后续接入状态预测、动作价值、好奇心、进化策略或 Offline RL。
- 不包含 self-attention、词表、next-token 解码或文本生成核心链路。

## MSVC 构建

使用 Visual Studio 2022 的 **Developer PowerShell**：

```powershell
.\build-msvc.ps1
```

或直接打开 `Osten.sln`，选择 `Release | x64` 后生成。输出：

```text
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
Osten.sln        Visual Studio 解决方案
*.vcxproj        MSVC 项目文件
```

该版本是 0.1.4-alpha 的运行骨架，不宣称已经完成外部环境适配或训练收敛。
