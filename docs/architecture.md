# Osten 0.1.4-alpha 架构约束

本实现将 `todolist.md` 的硬点映射到代码：

| 硬点 | 实现 |
| --- | --- |
| 文本不能直接进入网络 | `CharacterNGramEncoder::encode` 先将字符串转换为固定维度向量 |
| 关键词/规则不能直接决策 | `OstenEngine::tick` 只将编码向量、当前状态、目标、记忆摘要和 heartbeat 交给 `ForwardPolicy` |
| 规则仅作边界保护 | `math::clip_finite` 只处理 NaN、Inf 和数值范围，不理解文本或动作语义 |
| 记忆只供给上下文 | `EpisodicMemory::summary` 生成加权向量，不返回答案或动作 |
| 禁止 Transformer/self-attention | `ForwardPolicy` 是固定尺寸的两层 gated MLP |
| 禁止 next-token | 网络输出 action/state/memory/goal 字段，没有词表、token logits 或解码循环 |
| 无输入也必须运转 | `OstenEngine::heartbeat` 每个 tick 生成周期与进度信号 |
| 固定规模 | `Config` 在构造时确定全部层尺寸，运行中不扩容 |
| 训练目标偏向行动 | `TrainingSample` 保留 state/action/next_state/reward 接口，训练模块不承担文本预测 |
| 基线评测 | `evaluate_baseline` 覆盖存活、多样性、记忆、目标、自一致性和行动连续性指标 |

`Decision` 是核心输出载体：`state_delta`、`action_id`、记忆写入强度和
`goal_delta` 都来自同一次网络前向。当前版本暂不提供文本输出，因为文本不是
Osten 的核心证明。
