# Qt UI美化最佳实践指南

## 设计原则

### 1. 一致性原则
- **视觉一致性**: 整个应用使用统一的色彩、字体、间距规范
- **交互一致性**: 相同功能的行为方式应该保持一致
- **布局一致性**: 使用网格系统和统一的间距标准

### 2. 层次性原则
- **信息层次**: 通过大小、颜色、位置建立清晰的视觉层次
- **功能层次**: 主要功能突出显示，次要功能适当弱化
- **操作层次**: 常用操作更容易访问，复杂操作有引导

### 3. 简洁性原则
- **减少视觉噪音**: 移除不必要的装饰和元素
- **功能优先**: 界面设计服务于功能，不为了美观而美观
- **清晰明确**: 用户一眼就能理解界面的用途和操作方式

## 色彩系统

### 色彩搭配原则

#### 主色调选择
```css
/* 推荐的主色调 */
专业蓝色: #2196F3 (Material Blue)
科技绿色: #4CAF50 (Material Green)
现代紫色: #9C27B0 (Material Purple)
活力橙色: #FF5722 (Material Deep Orange)
```

#### 色彩搭配方案
```css
/* 蓝色主题配色方案 */
--primary: #2196F3;          /* 主色 */
--primary-dark: #1976D2;     /* 主色深色 */
--primary-light: #42A5F5;    /* 主色浅色 */
--accent: #FF5722;           /* 强调色 */
--background: #FAFAFA;       /* 背景色 */
--surface: #FFFFFF;          /* 表面色 */
--text-primary: #37474F;     /* 主要文字 */
--text-secondary: #616161;   /* 次要文字 */
--border: #E0E0E0;           /* 边框色 */
```

#### 对比度要求
- **正文文字**: 对比度至少 4.5:1
- **大标题文字**: 对比度至少 3:1
- **非文字元素**: 对比度至少 3:1

### 色彩使用规范

#### 成功、警告、错误状态
```css
/* 状态色彩 */
--success: #4CAF50;    /* 成功状态 */
--warning: #FF9800;    /* 警告状态 */
--error: #F44336;      /* 错误状态 */
--info: #2196F3;       /* 信息状态 */
```

#### 中性色彩
```css
/* 灰度色彩 */
--gray-50: #FAFAFA;
--gray-100: #F5F5F5;
--gray-200: #EEEEEE;
--gray-300: #E0E0E0;
--gray-400: #BDBDBD;
--gray-500: #9E9E9E;
--gray-600: #757575;
--gray-700: #616161;
--gray-800: #424242;
--gray-900: #212121;
```

## 字体系统

### 字体选择原则

#### 中文字体优先级
```css
font-family: "PingFang SC", "Microsoft YaHei", "微软雅黑",
             "Helvetica Neue", Arial, sans-serif;
```

#### 英文字体优先级
```css
font-family: "Segoe UI", "Roboto", "Helvetica Neue", Arial, sans-serif;
```

### 字体大小规范
```css
/* 字体大小层级 */
--font-size-xs: 12px;    /* 辅助信息 */
--font-size-sm: 14px;    /* 正文 */
--font-size-base: 16px;  /* 基础字号 */
--font-size-lg: 18px;    /* 小标题 */
--font-size-xl: 24px;    /* 标题 */
--font-size-2xl: 32px;   /* 大标题 */
--font-size-3xl: 48px;   /* 超大标题 */
```

### 字重规范
```css
/* 字重使用 */
font-weight: 300;        /* 细体 - 大标题 */
font-weight: 400;        /* 常规 - 正文 */
font-weight: 500;        /* 中等 - 强调 */
font-weight: 600;        /* 半粗 - 小标题 */
font-weight: 700;        /* 粗体 - 按钮文字 */
```

## 间距系统

### 基础间距单位
```css
/* 间距系统 (基于8px网格) */
--spacing-xs: 4px;       /* 极小间距 */
--spacing-sm: 8px;       /* 小间距 */
--spacing-md: 16px;      /* 中等间距 */
--spacing-lg: 24px;      /* 大间距 */
--spacing-xl: 32px;      /* 超大间距 */
--spacing-2xl: 48px;     /* 特大间距 */
```

### 间距使用原则
- **组件内部**: 使用 4px, 8px
- **组件之间**: 使用 16px, 24px
- **区块之间**: 使用 32px, 48px

## 组件设计规范

### 按钮设计

#### 主要按钮
```css
QPushButton[class="primary"] {
    background: var(--primary);
    color: white;
    border: none;
    border-radius: 6px;
    padding: 8px 16px;
    font-weight: 600;
    min-height: 32px;
}

QPushButton[class="primary"]:hover {
    background: var(--primary-dark);
    transform: translateY(-1px);
    box-shadow: 0 4px 8px rgba(0,0,0,0.12);
}
```

#### 次要按钮
```css
QPushButton[class="secondary"] {
    background: transparent;
    color: var(--primary);
    border: 2px solid var(--primary);
    border-radius: 6px;
    padding: 6px 14px;
    font-weight: 500;
}

QPushButton[class="secondary"]:hover {
    background: var(--primary);
    color: white;
}
```

#### 危险按钮
```css
QPushButton[class="danger"] {
    background: var(--error);
    color: white;
    border: none;
    border-radius: 6px;
    padding: 8px 16px;
    font-weight: 600;
}
```

### 输入框设计

#### 基础输入框
```css
QLineEdit {
    border: 2px solid var(--border);
    border-radius: 6px;
    padding: 8px 12px;
    font-size: 14px;
    background: var(--surface);
    color: var(--text-primary);
    transition: border-color 0.2s;
}

QLineEdit:focus {
    border-color: var(--primary);
    outline: none;
    box-shadow: 0 0 0 3px rgba(33, 150, 243, 0.1);
}

QLineEdit:disabled {
    background: var(--gray-100);
    color: var(--text-secondary);
    border-color: var(--gray-300);
}
```

#### 错误状态
```css
QLineEdit[class="error"] {
    border-color: var(--error);
}

QLineEdit[class="error"]:focus {
    box-shadow: 0 0 0 3px rgba(244, 67, 54, 0.1);
}
```

### 卡片设计

#### 基础卡片
```css
QWidget[class="card"] {
    background: var(--surface);
    border-radius: 8px;
    border: 1px solid var(--border);
    box-shadow: 0 2px 4px rgba(0,0,0,0.08);
    padding: var(--spacing-lg);
}

QWidget[class="card"]:hover {
    box-shadow: 0 4px 8px rgba(0,0,0,0.12);
    transform: translateY(-1px);
}
```

## 性能优化

### 样式表性能优化

#### 选择器优化
```css
/* ✅ 推荐使用类选择器 */
.my-button {
    background: var(--primary);
}

/* ❌ 避免过于具体的选择器 */
QMainWindow QWidget QPushButton {
    background: var(--primary);
}

/* ✅ 使用直接子选择器 */
QWidget > QPushButton {
    background: var(--primary);
}
```

#### 属性优化
```css
/* ✅ 合并相关属性 */
QPushButton {
    background: var(--primary);
    color: white;
    border: none;
    border-radius: 6px;
    padding: 8px 16px;
}

/* ❌ 避免重复定义 */
QPushButton { background: var(--primary); }
QPushButton { color: white; }
QPushButton { border: none; }
```

### 绘制性能优化

#### 减少重绘
```cpp
// ✅ 使用缓存
class OptimizedWidget : public QWidget {
public:
    void paintEvent(QPaintEvent *event) override {
        if (cache.isNull()) {
            cache = QPixmap(size());
            cache.fill(Qt::transparent);

            QPainter painter(&cache);
            drawContent(&painter);
        }

        QPainter painter(this);
        painter.drawPixmap(0, 0, cache);
    }

private:
    QPixmap cache;
};
```

#### 避免不必要的透明度
```css
/* ❌ 避免在性能敏感区域使用 */
QScrollBar {
    background: rgba(255,255,255,0.95); /* 透明度消耗性能 */
}

/* ✅ 使用实色 */
QScrollBar {
    background: var(--surface);
}
```

## 响应式设计

### 弹性布局
```cpp
// 使用布局管理器
void setupResponsiveLayout() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // 设置拉伸因子
    mainLayout->setStretchFactor(leftPanel, 1);
    mainLayout->setStretchFactor(contentArea, 3);
    mainLayout->setStretchFactor(rightPanel, 1);

    // 设置边距和间距
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);
}
```

### 自适应字体大小
```cpp
// 根据窗口大小调整字体
bool eventFilter(QObject* obj, QEvent* event) override {
    if (event->type() == QEvent::Resize) {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        int fontSize = qMax(12, resizeEvent->size().width() / 80);
        QFont font = this->font();
        font.setPointSize(fontSize);
        this->setFont(font);
    }
    return QObject::eventFilter(obj, event);
}
```

## 可访问性设计

### 颜色可访问性
```css
/* 确保足够的对比度 */
QLabel {
    color: var(--text-primary);    /* 与背景对比度 > 4.5:1 */
    background: var(--background);
}

/* 不仅依赖颜色传达信息 */
QPushButton[class="error"] {
    background: var(--error);
    border: 2px solid #D32F2F;  /* 添加边框强化视觉提示 */
    font-weight: 600;             /* 使用字重强调 */
}
```

### 键盘导航支持
```cpp
// 确保所有交互元素可通过Tab键访问
button->setFocusPolicy(Qt::TabFocus);
lineEdit->setFocusPolicy(Qt::StrongFocus);

// 设置Tab顺序
setTabOrder(button1, button2);
setTabOrder(button2, lineEdit);
```

### 焦点指示器
```css
/* 清晰的焦点指示器 */
QWidget:focus {
    outline: 2px solid var(--primary);
    outline-offset: 2px;
}

/* 为不同控件设置焦点样式 */
QLineEdit:focus {
    border: 2px solid var(--primary);
    box-shadow: 0 0 0 3px rgba(33, 150, 243, 0.2);
}
```

## 常见错误避免

### 颜色使用错误
```css
/* ❌ 使用过多颜色 */
.button1 { background: #FF0000; }
.button2 { background: #00FF00; }
.button3 { background: #0000FF; }
.button4 { background: #FFFF00; }

/* ✅ 使用统一的色彩系统 */
--primary: #2196F3;
--success: #4CAF50;
--warning: #FF9800;
--error: #F44336;
```

### 尺寸不一致
```css
/* ❌ 不一致的间距 */
.button1 { margin: 5px; }
.button2 { margin: 8px; }
.button3 { margin: 10px; }

/* ✅ 使用间距系统 */
.button {
    margin: var(--spacing-sm);
    padding: var(--spacing-sm) var(--spacing-md);
}
```

### 过度装饰
```css
/* ❌ 过度的装饰效果 */
.button {
    background: linear-gradient(45deg, #FF0000, #00FF00, #0000FF);
    border-radius: 20px;
    box-shadow: 0 10px 20px rgba(0,0,0,0.5);
    transform: rotate(1deg);
    animation: pulse 2s infinite;
}

/* ✅ 简洁有效的设计 */
.button {
    background: var(--primary);
    border-radius: 6px;
    transition: transform 0.2s;
}

.button:hover {
    transform: translateY(-1px);
    box-shadow: 0 4px 8px rgba(0,0,0,0.12);
}
```

## 测试与验证

### 视觉测试清单
- [ ] 所有颜色对比度符合WCAG标准
- [ ] 不同DPI设置下显示正常
- [ ] 深色/浅色主题切换正常
- [ ] 不同屏幕尺寸下布局正确
- [ ] 焦点指示器清晰可见
- [ ] 动画效果流畅不卡顿

### 性能测试
- [ ] 样式表应用时间 < 100ms
- [ ] 窗口切换无明显延迟
- [ ] 大量控件更新时保持流畅
- [ ] 内存使用稳定无泄漏

### 兼容性测试
- [ ] Windows 10/11 兼容性
- [ ] macOS 10.15+ 兼容性
- [ ] 主流Linux发行版兼容性
- [ ] 不同Qt版本兼容性

通过遵循这些最佳实践，可以创建出美观、高效、易用的Qt应用程序界面。