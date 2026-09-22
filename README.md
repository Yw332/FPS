# 🎯 PyramidKillTrial

> 基于 Unreal Engine 5 + C++ 独立开发的第一人称限时射击挑战 Demo

![PyramidKillTrial](docs/images/main_menu.png)

## 📌 项目简介

**PyramidKillTrial** 是一个基于 **UE5 + C++**
独立开发的第一人称限时射击游戏 Demo。

玩家需要在 **90 秒**内击败尽可能多的敌人，根据最终击杀数量获得 **S / A /
B / C / D** 五档评级。

如果玩家在挑战过程中死亡，则立即结束本局并进入 **Mission Failed** 结算。

项目主要用于实践 UE5 C++
游戏客户端开发，包括角色、武器、战斗、GAS、AI、UI、动画以及游戏流程等系统。

------------------------------------------------------------------------

## 🎮 核心玩法

``` text
进入游戏
   ↓
查看操作说明
   ↓
开始 90 秒挑战
   ↓
敌人持续生成
   ↓
移动 / 瞄准 / 射击 / 换弹
   ↓
击杀尽可能多的敌人
   ↓
计时结束
   ↓
根据击杀数计算评级
```

### 主要功能

-   ⏱️ 90 秒限时挑战
-   🔫 第一人称武器系统
-   🎯 瞄准 / ADS
-   🔄 换弹系统
-   🔀 半自动 / 全自动射击模式切换
-   🤖 敌人巡逻、感知、追击与攻击
-   ❤️ 玩家与敌人生命值系统
-   👾 敌人动态生成
-   📊 实时击杀数与倒计时
-   🏆 S / A / B / C / D 五档评级
-   💀 玩家死亡触发 Mission Failed
-   ✨ Niagara 战斗特效
-   🔊 MetaSound 战斗音效
-   🖥️ 开始界面、教程界面、战斗 HUD、结算界面

------------------------------------------------------------------------

# 📸 游戏截图

## 主菜单

![主菜单](docs/images/main_menu.png)

玩家可以从主菜单开始挑战或退出游戏。

## 操作说明

![操作说明](docs/images/tutorial.png)

进入挑战前展示移动、跳跃、冲刺、开火、瞄准等基本操作。

## 战斗场景

![战斗场景](docs/images/combat.png)

第一人称战斗视角，包括武器、准星、生命值、弹药以及倒计时等 HUD 信息。

## 多敌人战斗

![多敌人战斗](docs/images/combat_multiple.png)

挑战过程中会持续生成敌人，玩家需要在有限时间内尽可能提高击杀数量。

## 最终结算

![最终结算](docs/images/mission_complete.png)

计时结束后显示最终击杀数和评级，并提供重新开始或返回菜单的选项。

------------------------------------------------------------------------

# 🕹️ 操作方式

  按键           功能
  -------------- ---------------------
  `W A S D`      移动
  `Space`        跳跃
  `Left Shift`   冲刺
  `鼠标左键`     开火
  `鼠标右键`     瞄准 / ADS
  `R`            换弹
  `B`            切换半自动 / 全自动

------------------------------------------------------------------------

# ⚙️ 技术实现

## 1. C++ + Blueprint 混合架构

项目采用 **C++ + Blueprint** 混合开发方式。

C++ 主要负责：

-   角色核心逻辑
-   武器系统
-   开火与换弹
-   射击检测
-   GAS 属性与伤害
-   AI 攻击判定
-   游戏流程核心逻辑

Blueprint 主要用于：

-   Gameplay 流程编排
-   AI Behavior Tree 配置
-   UI 与表现逻辑
-   动画蓝图
-   部分资源与场景配置

通过这种方式将核心 Gameplay 逻辑与表现层进行一定程度的分离。

------------------------------------------------------------------------

## 2. 两阶段射线检测

武器开火并不是简单地从摄像机直接进行一次射线检测，而是采用：

``` text
第一阶段：Camera Trace
        ↓
获取准星对应的 AimPoint
        ↓
第二阶段：Weapon Trace
        ↓
从枪口向 AimPoint 发射射线
        ↓
获取最终命中目标
```

第一阶段负责确定玩家实际瞄准的位置。

第二阶段从武器枪口向 AimPoint
进行检测，用于模拟第一人称武器实际的子弹路径，同时处理摄像机与枪口之间的位置偏移。

------------------------------------------------------------------------

## 3. Gameplay Ability System

项目使用 **Gameplay Ability System（GAS）**构建属性与伤害体系。

核心流程：

``` text
武器开火
   ↓
射线命中
   ↓
获取目标 Actor
   ↓
获取目标 ASC
   ↓
创建 / 应用 GameplayEffect
   ↓
修改 AttributeSet
   ↓
生命值变化
   ↓
死亡判定
```

其中：

-   `AbilitySystemComponent` 负责 GAS 核心能力
-   `AttributeSet` 管理角色属性
-   `GameplayEffect` 用于处理伤害等属性修改
-   Gameplay Attribute 变化通过委托同步到 UI

玩家 ASC 挂载在 `PlayerState`，敌人 ASC 挂载在敌人 Character 上。

------------------------------------------------------------------------

## 4. 敌人 AI

敌人 AI 使用：

-   Behavior Tree
-   Blackboard
-   Pawn Sensing

实现基本行为：

``` text
巡逻
 ↓
感知玩家
 ↓
追击
 ↓
攻击
 ↓
玩家离开感知范围
 ↓
返回巡逻
```

项目中设计了不同敌人类型，并为其配置不同的攻击行为。

近战攻击通过：

``` text
AnimMontage
     ↓
AnimNotify
     ↓
C++ 球形检测
     ↓
命中目标
     ↓
造成伤害
```

实现动画与攻击判定之间的同步。

------------------------------------------------------------------------

## 5. GameMode 游戏流程管理

游戏核心流程由 `GameMode` 统一管理：

``` text
GameMode
├── 挑战计时
├── 敌人生成
├── SpawnPoint 管理
├── 场上敌人数控制
├── 击杀统计
├── 评级计算
└── 游戏结束
```

挑战过程中通过 Timer 动态生成敌人，并使用预先放置的 SpawnPoint
作为敌人出生位置。

同时限制场上敌人数量，避免敌人无限生成造成不必要的性能压力。

------------------------------------------------------------------------

## 6. HUD / Widget / WidgetController

项目采用类似：

``` text
Gameplay Data
     ↓
Widget Controller
     ↓
HUD Widget
```

的数据流结构。

其中：

### GAS

负责：

-   Health
-   MaxHealth
-   属性变化

### Weapon

负责：

-   CurrentAmmo
-   MagazineCapacity
-   ReserveAmmo
-   Reload State

### Widget Controller

负责将 Gameplay 数据传递给 UI。

### HUD

负责最终表现：

-   生命值
-   当前弹药
-   备用弹药
-   准星
-   ADS 状态
-   Low Ammo 提示
-   换弹提示
-   剩余时间
-   击杀数量
-   最终评级

UI 主要通过事件 / 委托驱动更新，而不是依赖 Tick 持续查询。

------------------------------------------------------------------------

# 🧠 技术架构

``` text
                   Enhanced Input
                         │
                         ▼
                ┌─────────────────┐
                │ Player Character│
                └────────┬────────┘
                         │
             ┌───────────┼───────────┐
             ▼           ▼           ▼
          Weapon        GAS        Camera
             │           │           │
             ▼           ▼           ▼
       Weapon Trace  AttributeSet   ADS
             │           │
             └─────┬─────┘
                   ▼
              Combat Result
                   │
                   ▼
              WidgetController
                   │
                   ▼
                  HUD


GameMode
   │
   ├── Timer
   ├── Enemy Spawn
   ├── Kill Count
   └── Rating


Enemy
   │
   ├── Behavior Tree
   ├── Blackboard
   ├── Pawn Sensing
   └── AnimMontage / AnimNotify
```

------------------------------------------------------------------------

# 🛠️ 技术栈

  分类              技术
  ----------------- -------------------------------------------
  游戏引擎          Unreal Engine 5
  编程语言          C++
  可视化脚本        Blueprint
  属性 / 伤害系统   Gameplay Ability System
  输入              Enhanced Input
  AI                Behavior Tree / Blackboard / Pawn Sensing
  动画              Animation Montage / AnimNotify
  特效              Niagara
  音频              MetaSound
  版本管理          Git
  UE 大型资源管理   Git LFS

------------------------------------------------------------------------

# 📁 项目结构

``` text
FPS/
├── Config/
├── Content/
│   ├── Animation/
│   ├── Characters/
│   ├── Effects/
│   ├── UI/
│   ├── Weapons/
│   └── ...
│
├── Plugins/
│   └── UEFormat/
│
├── Source/
│   └── FPS/
│       ├── Character/
│       ├── Weapon/
│       ├── Ability/
│       ├── Attribute/
│       ├── AI/
│       └── ...
│
├── docs/
│   └── images/
│       ├── main_menu.png
│       ├── tutorial.png
│       ├── combat.png
│       ├── combat_multiple.png
│       └── mission_complete.png
│
├── .gitattributes
├── .gitignore
└── FPS.uproject
```

------------------------------------------------------------------------

# 📋 项目信息

  项目       内容
  ---------- ------------------
  项目名称   PyramidKillTrial
  开发时间   2026.08 - 至今
  开发者     Yw332
  开发方式   个人独立开发
  游戏引擎   Unreal Engine 5
  编程语言   C++
  游戏类型   第一人称限时射击

------------------------------------------------------------------------

# 🎯 项目目标

本项目主要用于实践 **UE5 C++ 游戏客户端开发**，重点学习和实现：

-   UE5 Gameplay Framework
-   C++ 与 Blueprint 混合开发
-   武器与战斗系统
-   Gameplay Ability System
-   Enemy AI
-   动画与 Gameplay 事件同步
-   HUD / Widget / WidgetController
-   Git / Git LFS 项目管理

------------------------------------------------------------------------

# 👨‍💻 Developer

**Yw332**

方向：**UE5 / C++ 游戏客户端开发**

关注：

`Gameplay Programming` · `Unreal Engine` · `C++` · `Combat System` ·
`Game AI`
