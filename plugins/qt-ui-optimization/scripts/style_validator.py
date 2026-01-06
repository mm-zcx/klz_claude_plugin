#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Qt样式表验证工具
检查Qt样式表语法错误和性能问题
"""

import re
import sys
import os
import argparse
from typing import List, Tuple, Dict
import json

class StyleValidationError:
    def __init__(self, error_type: str, line_number: int, column: int,
                 message: str, severity: str = "error"):
        self.error_type = error_type
        self.line_number = line_number
        self.column = column
        self.message = message
        self.severity = severity  # error, warning, info

    def to_dict(self) -> Dict:
        return {
            'type': self.error_type,
            'line': self.line_number,
            'column': self.column,
            'message': self.message,
            'severity': self.severity
        }

class QtStyleValidator:
    def __init__(self):
        self.errors = []
        self.warnings = []

        # Qt样式表有效属性列表
        self.valid_properties = {
            # 颜色和背景
            'color', 'background-color', 'background', 'background-image',
            'background-repeat', 'background-position', 'background-attachment',
            'background-clip', 'background-origin',

            # 字体
            'font', 'font-family', 'font-size', 'font-weight', 'font-style',
            'font-variant', 'text-decoration', 'text-transform',

            # 布局
            'width', 'height', 'min-width', 'min-height', 'max-width', 'max-height',
            'margin', 'margin-top', 'margin-right', 'margin-bottom', 'margin-left',
            'padding', 'padding-top', 'padding-right', 'padding-bottom', 'padding-left',
            'border', 'border-top', 'border-right', 'border-bottom', 'border-left',
            'border-color', 'border-style', 'border-width', 'border-radius',
            'border-image', 'border-top-left-radius', 'border-top-right-radius',
            'border-bottom-left-radius', 'border-bottom-right-radius',

            # 定位
            'position', 'top', 'right', 'bottom', 'left',
            'float', 'clear', 'display', 'visibility', 'z-index', 'opacity',

            # 特殊Qt属性
            'selection-background-color', 'selection-color',
            'alternate-background-color', 'gridline-color',
            'outline', 'outline-color', 'outline-style', 'outline-offset',

            # 子控件控制
            'subcontrol-origin', 'subcontrol-position', 'spacing',

            # 其他
            'icon-size', 'line-height', 'text-align', 'vertical-align'
        }

        # Qt样式表伪状态
        self.pseudo_states = {
            'hover', 'pressed', 'focus', 'disabled', 'checked', 'selected',
            'unchecked', 'indeterminate', 'readonly', 'enabled', 'active',
            'default', 'flat', 'open', 'closed', 'on', 'off', 'horizontal',
            'vertical', 'next-selected', 'previous-selected', 'selected',
            'first', 'last', 'middle', 'only-one', 'item', 'sibling',
            'top', 'bottom', 'left', 'right', 'no-frame', 'window',
            'dialog', 'popup', 'modal', 'non-modal'
        }

        # Qt控件类型
        self.qt_widgets = {
            'QWidget', 'QMainWindow', 'QPushButton', 'QLabel', 'QLineEdit',
            'QTextEdit', 'QPlainTextEdit', 'QComboBox', 'QCheckBox', 'QRadioButton',
            'QSlider', 'QProgressBar', 'QTableWidget', 'QTableView', 'QTreeView',
            'QTreeWidget', 'QListWidget', 'QTabWidget', 'QTabBar', 'QMenuBar',
            'QMenu', 'QToolBar', 'QStatusBar', 'QScrollBar', 'QGroupBox',
            'QDialog', 'QMessageBox', 'QToolTip', 'QSplitter', 'QCalendarWidget',
            'QDateTimeEdit', 'QSpinBox', 'QDoubleSpinBox', 'QHeaderView'
        }

    def validate_file(self, file_path: str, strict_mode: bool = False) -> List[StyleValidationError]:
        """验证样式表文件"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
        except Exception as e:
            return [StyleValidationError(
                "file_error", 0, 0, f"无法读取文件: {e}", "error"
            )]

        return self.validate_content(content, strict_mode)

    def validate_content(self, content: str, strict_mode: bool = False) -> List[StyleValidationError]:
        """验证样式表内容"""
        self.errors = []
        self.warnings = []

        # 预处理：移除注释
        content_without_comments = self._remove_comments(content)

        # 基本语法检查
        self._check_basic_syntax(content_without_comments)

        # 属性验证
        self._validate_properties(content_without_comments, strict_mode)

        # 选择器验证
        self._validate_selectors(content_without_comments, strict_mode)

        # 性能检查
        self._check_performance(content_without_comments)

        # 值验证
        self._validate_values(content_without_comments)

        return self.errors + self.warnings

    def _remove_comments(self, content: str) -> str:
        """移除CSS注释"""
        # 移除多行注释 /* ... */
        return re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

    def _check_basic_syntax(self, content: str):
        """检查基本语法错误"""
        lines = content.split('\n')
        bracket_stack = []

        for line_num, line in enumerate(lines, 1):
            line = line.strip()
            if not line or line.startswith('/*'):
                continue

            # 检查大括号匹配
            for col, char in enumerate(line, 1):
                if char == '{':
                    bracket_stack.append((line_num, col))
                elif char == '}':
                    if not bracket_stack:
                        self.errors.append(StyleValidationError(
                            "syntax_error", line_num, col, "不匹配的右大括号", "error"
                        ))
                    else:
                        bracket_stack.pop()

            # 检查行末分号
            if '{' not in line and '}' not in line and ':' in line and line.strip() != '*/':
                if not line.strip().endswith(';') and not line.strip().endswith('{'):
                    # 检查是否是最后一行或包含其他特殊情况
                    self.warnings.append(StyleValidationError(
                        "missing_semicolon", line_num, len(line),
                        "属性行末尾缺少分号", "warning"
                    ))

        # 检查未闭合的大括号
        for line_num, col in bracket_stack:
            self.errors.append(StyleValidationError(
                "unclosed_bracket", line_num, col, "未闭合的左大括号", "error"
            ))

    def _validate_properties(self, content: str, strict_mode: bool):
        """验证CSS属性"""
        # 匹配属性定义
        property_pattern = r'([a-zA-Z-]+)\s*:\s*([^;]+);'

        for match in re.finditer(property_pattern, content):
            property_name = match.group(1)
            line_num = content[:match.start()].count('\n') + 1
            col = match.start() - content.rfind('\n', 0, match.start())

            if strict_mode and property_name not in self.valid_properties:
                self.warnings.append(StyleValidationError(
                    "unknown_property", line_num, col,
                    f"未知属性: {property_name}", "warning"
                ))

    def _validate_selectors(self, content: str, strict_mode: bool):
        """验证选择器"""
        # 匹配选择器
        selector_pattern = r'([^{]+)\s*\{'

        for match in re.finditer(selector_pattern, content):
            selector = match.group(1).strip()
            line_num = content[:match.start()].count('\n') + 1
            col = match.start() - content.rfind('\n', 0, match.start())

            # 检查伪状态
            for part in selector.split(':'):
                if part and part not in self.qt_widgets and not part.startswith('::'):
                    # 可能是伪状态
                    if part not in self.pseudo_states:
                        self.warnings.append(StyleValidationError(
                            "unknown_pseudo_state", line_num, col,
                            f"未知伪状态: :{part}", "warning"
                        ))

    def _check_performance(self, content: str):
        """检查性能问题"""
        lines = content.split('\n')

        for line_num, line in enumerate(lines, 1):
            line = line.strip()

            # 检查过于具体的选择器
            if ' '.join(line.split()[:5]).count(' ') > 3 and ':' not in line:
                self.warnings.append(StyleValidationError(
                    "complex_selector", line_num, 1,
                    "选择器过于复杂，可能影响性能", "warning"
                ))

            # 检查通配符选择器
            if line.strip() == '*' and '{' in line:
                self.warnings.append(StyleValidationError(
                    "universal_selector", line_num, 1,
                    "避免使用通配符选择器，会影响性能", "warning"
                ))

    def _validate_values(self, content: str):
        """验证属性值"""
        # 颜色值验证
        color_pattern = r'#[0-9a-fA-F]{3,6}'
        for match in re.finditer(color_pattern, content):
            color = match.group()
            if len(color) == 4:  # #RGB
                pass  # 简写颜色是有效的
            elif len(color) == 7:  # #RRGGBB
                pass  # 完整颜色是有效的
            else:
                line_num = content[:match.start()].count('\n') + 1
                self.warnings.append(StyleValidationError(
                    "invalid_color", line_num, match.start(),
                    f"无效的颜色值: {color}", "warning"
                ))

    def generate_report(self, errors: List[StyleValidationError],
                       output_format: str = "text") -> str:
        """生成验证报告"""
        if output_format == "json":
            return json.dumps([error.to_dict() for error in errors],
                            indent=2, ensure_ascii=False)

        # 文本格式报告
        report = []
        report.append("=" * 50)
        report.append("Qt样式表验证报告")
        report.append("=" * 50)

        error_count = sum(1 for e in errors if e.severity == "error")
        warning_count = sum(1 for e in errors if e.severity == "warning")

        report.append(f"错误: {error_count}")
        report.append(f"警告: {warning_count}")
        report.append(f"总计: {len(errors)}")
        report.append("")

        if not errors:
            report.append("✅ 样式表验证通过！")
        else:
            # 按严重程度分组
            errors_by_severity = {"error": [], "warning": [], "info": []}
            for error in errors:
                errors_by_severity[error.severity].append(error)

            for severity in ["error", "warning", "info"]:
                if errors_by_severity[severity]:
                    severity_name = {"error": "错误", "warning": "警告", "info": "信息"}[severity]
                    report.append(f"\n{severity_name.upper()}:")
                    report.append("-" * 30)

                    for error in errors_by_severity[severity]:
                        report.append(f"行 {error.line_number}:{error.column} - {error.message}")

        return "\n".join(report)

def main():
    parser = argparse.ArgumentParser(description='Qt样式表验证工具')
    parser.add_argument('style_file', help='样式表文件路径')
    parser.add_argument('--output', help='报告输出文件路径')
    parser.add_argument('--format', choices=['text', 'json'], default='text',
                       help='报告格式 (默认: text)')
    parser.add_argument('--strict', action='store_true',
                       help='严格模式检查')
    parser.add_argument('--fix-auto', action='store_true',
                       help='自动修复常见问题')

    args = parser.parse_args()

    if not os.path.exists(args.style_file):
        print(f"❌ 文件不存在: {args.style_file}")
        sys.exit(1)

    validator = QtStyleValidator()
    errors = validator.validate_file(args.style_file, args.strict)

    # 生成报告
    report = validator.generate_report(errors, args.format)

    if args.output:
        with open(args.output, 'w', encoding='utf-8') as f:
            f.write(report)
        print(f"✅ 报告已保存到: {args.output}")
    else:
        print(report)

    # 返回适当的退出码
    error_count = sum(1 for e in errors if e.severity == "error")
    if error_count > 0:
        sys.exit(1)
    else:
        sys.exit(0)

if __name__ == '__main__':
    main()