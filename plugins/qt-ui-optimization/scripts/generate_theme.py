#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Qt主题生成器 - 自动生成自定义Qt样式表
支持多种颜色方案和自定义配置
"""

import argparse
import os
import sys
from typing import Dict, List, Tuple
import json

class ThemeGenerator:
    def __init__(self):
        self.color_schemes = {
            'blue': {
                'primary': '#2196F3',
                'primary_dark': '#1976D2',
                'primary_light': '#42A5F5',
                'accent': '#FF5722',
                'background': '#FAFAFA',
                'surface': '#FFFFFF',
                'text_primary': '#37474F',
                'text_secondary': '#616161',
                'border': '#E0E0E0',
                'shadow': 'rgba(0,0,0,0.12)'
            },
            'green': {
                'primary': '#4CAF50',
                'primary_dark': '#388E3C',
                'primary_light': '#66BB6A',
                'accent': '#FF9800',
                'background': '#FAFAFA',
                'surface': '#FFFFFF',
                'text_primary': '#37474F',
                'text_secondary': '#616161',
                'border': '#E0E0E0',
                'shadow': 'rgba(0,0,0,0.12)'
            },
            'purple': {
                'primary': '#9C27B0',
                'primary_dark': '#7B1FA2',
                'primary_light': '#AB47BC',
                'accent': '#FF5722',
                'background': '#FAFAFA',
                'surface': '#FFFFFF',
                'text_primary': '#37474F',
                'text_secondary': '#616161',
                'border': '#E0E0E0',
                'shadow': 'rgba(0,0,0,0.12)'
            },
            'dark': {
                'primary': '#007ACC',
                'primary_dark': '#005A9E',
                'primary_light': '#1E8AC7',
                'accent': '#FF6B35',
                'background': '#2D2D30',
                'surface': '#3C3C3C',
                'text_primary': '#E0E0E0',
                'text_secondary': '#A0A0A0',
                'border': '#555555',
                'shadow': 'rgba(0,0,0,0.4)'
            },
            'military': {
                'primary': '#4A5F4A',  # 军绿色
                'primary_dark': '#3D4A3D',  # 深军绿
                'primary_light': '#5A6F5A',  # 浅军绿
                'accent': '#FF6B35',  # 战术橙
                'tactical': '#2ECC71',  # HUD绿
                'secondary': '#3498DB',  # 科技蓝
                'background': '#1A1A1A',  # 深黑背景
                'surface': 'rgba(26, 26, 26, 0.9)',  # 半透明表面
                'text_primary': '#E0E0E0',  # 主文字色
                'text_secondary': '#C3B091',  # 卡其色文字
                'border': '#8B7355',  # 橄榄褐边框
                'shadow': 'rgba(0,0,0,0.6)',  # 深色阴影
                'highlight': 'rgba(46, 204, 113, 0.8)',  # 荧光绿高亮
                'warning': '#FF6B35',  # 警告橙色
                'success': '#2ECC71',  # 成功绿色
                'info': '#3498DB'  # 信息蓝色
            }
        }

    def generate_theme(self, name: str, colors: Dict[str, str],
                      style: str = 'modern', output_path: str = None) -> str:
        """生成主题样式表"""

        template = self._get_theme_template(style)

        # 替换颜色占位符
        theme_css = template.format(**colors)

        # 添加主题头部注释
        header = f"""/*
 * {name} - Qt主题样式表
 * 由Qt主题生成器自动生成
 * 生成时间: {self._get_current_time()}
 */

"""

        full_theme = header + theme_css

        # 保存到文件
        if output_path:
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            with open(output_path, 'w', encoding='utf-8') as f:
                f.write(full_theme)
            print(f"✅ 主题已保存到: {output_path}")

        return full_theme

    def _get_theme_template(self, style: str) -> str:
        """获取主题模板"""

        if style == 'modern':
            return self._get_modern_template()
        elif style == 'minimal':
            return self._get_minimal_template()
        elif style == 'military':
            return self._get_military_template()
        else:
            return self._get_modern_template()

    def _get_modern_template(self) -> str:
        """现代风格模板"""
        return """
/* 全局基础设置 */
QWidget {{
    font-family: "Segoe UI", "Microsoft YaHei", Arial, sans-serif;
    font-size: 14px;
    color: {text_primary};
    background-color: {background};
    outline: 0px;
}}

/* 按钮样式 */
QPushButton {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary_light}, stop:1 {primary});
    border: none;
    border-radius: 6px;
    color: white;
    padding: 8px 16px;
    font-weight: 500;
    min-width: 80px;
    min-height: 32px;
}}

QPushButton:hover {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary}, stop:1 {primary_dark});
    box-shadow: 0 2px 4px {shadow};
}}

QPushButton:pressed {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary_dark}, stop:1 {primary_dark});
}}

QPushButton:disabled {{
    background-color: #E0E0E0;
    color: #9E9E9E;
    box-shadow: none;
}}

/* 主要按钮 */
QPushButton[class="primary"] {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary}, stop:1 {primary_dark});
    font-weight: 600;
}}

QPushButton[class="secondary"] {{
    background: transparent;
    border: 2px solid {primary};
    color: {primary};
}}

QPushButton[class="secondary"]:hover {{
    background: {primary};
    color: white;
}}

/* 输入框样式 */
QLineEdit, QTextEdit, QPlainTextEdit {{
    border: 2px solid {border};
    border-radius: 6px;
    padding: 8px 12px;
    background-color: {surface};
    selection-background-color: {primary};
}}

QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {{
    border-color: {primary};
    outline: none;
}}

QLineEdit:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {{
    background-color: {background};
    color: {text_secondary};
}}

/* 下拉框 */
QComboBox {{
    border: 2px solid {border};
    border-radius: 6px;
    padding: 8px 12px;
    background-color: {surface};
    min-width: 80px;
}}

QComboBox:hover {{
    border-color: {primary_light};
}}

QComboBox:focus {{
    border-color: {primary};
}}

QComboBox::drop-down {{
    border: none;
    width: 30px;
}}

QComboBox QAbstractItemView {{
    border: 1px solid {border};
    background-color: {surface};
    selection-background-color: {primary};
    selection-color: white;
    border-radius: 6px;
    padding: 4px;
}}

/* 复选框和单选框 */
QCheckBox, QRadioButton {{
    spacing: 8px;
    color: {text_primary};
}}

QCheckBox::indicator, QRadioButton::indicator {{
    width: 18px;
    height: 18px;
    border-radius: 3px;
    border: 2px solid {border};
    background-color: {surface};
}}

QCheckBox::indicator:hover, QRadioButton::indicator:hover {{
    border-color: {primary_light};
}}

QCheckBox::indicator:checked, QRadioButton::indicator:checked {{
    background-color: {primary};
    border-color: {primary};
}}

QRadioButton::indicator {{
    border-radius: 9px;
}}

/* 滑块 */
QSlider::groove:horizontal {{
    border: 1px solid {border};
    height: 8px;
    background: {background};
    border-radius: 4px;
}}

QSlider::handle:horizontal {{
    background: {primary};
    border: 2px solid {surface};
    width: 18px;
    margin: -7px 0;
    border-radius: 9px;
    box-shadow: 0 2px 4px {shadow};
}}

QSlider::handle:horizontal:hover {{
    background: {primary_dark};
}}

/* 进度条 */
QProgressBar {{
    border: none;
    border-radius: 4px;
    text-align: center;
    font-weight: 500;
    background-color: {border};
}}

QProgressBar::chunk {{
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 {primary_light}, stop:1 {primary});
    border-radius: 4px;
}}

/* 表格 */
QTableWidget, QTableView {{
    gridline-color: {border};
    background-color: {surface};
    alternate-background-color: {background};
    border-radius: 6px;
    selection-background-color: {primary_light};
}}

QTableWidget::item, QTableView::item {{
    padding: 8px;
    border-bottom: 1px solid {background};
}}

QTableWidget::item:selected, QTableView::item:selected {{
    background-color: {primary_light};
    color: {primary_dark};
}}

QHeaderView::section {{
    background-color: {background};
    padding: 8px 12px;
    border: none;
    border-right: 1px solid {border};
    border-bottom: 2px solid {primary};
    font-weight: 600;
    color: {text_primary};
}}

/* 选项卡 */
QTabWidget::pane {{
    border: 1px solid {border};
    border-radius: 6px;
    background-color: {surface};
}}

QTabBar::tab {{
    background-color: {background};
    border: none;
    padding: 10px 16px;
    margin-right: 4px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
}}

QTabBar::tab:selected {{
    background-color: {surface};
    border-bottom: 3px solid {primary};
    color: {primary};
    font-weight: 600;
}}

QTabBar::tab:hover {{
    background-color: {primary_light};
}}

/* 菜单 */
QMenuBar {{
    background-color: {background};
    border-bottom: 1px solid {border};
}}

QMenuBar::item {{
    padding: 8px 16px;
    background-color: transparent;
}}

QMenuBar::item:selected {{
    background-color: {primary_light};
    color: {primary};
}}

QMenu {{
    background-color: {surface};
    border: 1px solid {border};
    border-radius: 6px;
    padding: 4px;
}}

QMenu::item {{
    padding: 8px 16px;
    border-radius: 4px;
}}

QMenu::item:selected {{
    background-color: {primary_light};
    color: {primary};
}}

QMenu::separator {{
    height: 1px;
    background-color: {border};
    margin: 4px 8px;
}}

/* 工具栏 */
QToolBar {{
    background-color: {background};
    border: none;
    spacing: 4px;
    padding: 4px;
}}

/* 状态栏 */
QStatusBar {{
    background-color: {background};
    border-top: 1px solid {border};
    color: {text_secondary};
}}

/* 滚动条 */
QScrollBar:vertical {{
    background-color: {background};
    width: 12px;
    border-radius: 6px;
}}

QScrollBar::handle:vertical {{
    background-color: #BDBDBD;
    border-radius: 6px;
    min-height: 20px;
}}

QScrollBar::handle:vertical:hover {{
    background-color: #9E9E9E;
}}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {{
    height: 0px;
}}

QScrollBar:horizontal {{
    background-color: {background};
    height: 12px;
    border-radius: 6px;
}}

QScrollBar::handle:horizontal {{
    background-color: #BDBDBD;
    border-radius: 6px;
    min-width: 20px;
}}

QScrollBar::handle:horizontal:hover {{
    background-color: #9E9E9E;
}}

/* 工具提示 */
QToolTip {{
    background-color: {text_primary};
    color: white;
    border: none;
    border-radius: 4px;
    padding: 6px 10px;
    font-size: 13px;
}}

/* 分组框 */
QGroupBox {{
    font-weight: 600;
    border: 2px solid {border};
    border-radius: 8px;
    margin-top: 8px;
    padding-top: 16px;
    background-color: {surface};
}}

QGroupBox::title {{
    subcontrol-origin: margin;
    left: 8px;
    padding: 0 8px 0 8px;
    color: {primary};
}}
"""

    def _get_minimal_template(self) -> str:
        """极简风格模板"""
        return """
/* 极简风格主题 */
QWidget {{
    font-family: Arial, sans-serif;
    font-size: 13px;
    color: {text_primary};
    background-color: {surface};
}}

QPushButton {{
    background-color: {primary};
    border: none;
    border-radius: 3px;
    color: white;
    padding: 6px 12px;
    font-size: 13px;
}}

QPushButton:hover {{
    background-color: {primary_dark};
}}

QPushButton:disabled {{
    background-color: {border};
    color: {text_secondary};
}}

QLineEdit, QTextEdit {{
    border: 1px solid {border};
    border-radius: 3px;
    padding: 6px 8px;
    background-color: {surface};
}}

QLineEdit:focus, QTextEdit:focus {{
    border-color: {primary};
}}

QComboBox {{
    border: 1px solid {border};
    border-radius: 3px;
    padding: 6px 8px;
    background-color: {surface};
}}

QComboBox:focus {{
    border-color: {primary};
}}

QTabBar::tab {{
    background-color: {background};
    border: none;
    padding: 8px 12px;
}}

QTabBar::tab:selected {{
    background-color: {primary};
    color: white;
}}

QTableWidget {{
    background-color: {surface};
    alternate-background-color: {background};
    gridline-color: {border};
}}

QHeaderView::section {{
    background-color: {background};
    border: none;
    border-bottom: 1px solid {primary};
    padding: 6px 8px;
    font-weight: bold;
}}
"""

    def _get_military_template(self) -> str:
        """军工风格模板"""
        return """
/* 军工风格主题 - 军迷彩多色彩配置 */
QWidget {{
    font-family: "Consolas", "Monaco", "Microsoft YaHei", Arial, sans-serif;
    font-size: 14px;
    color: {text_primary};
    background-color: {background};
    outline: 0px;
}}

/* 主窗口 */
QMainWindow {{
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 {background}, stop:0.5 rgba(44, 44, 44, 0.8), stop:1 {background});
}}

/* 军工风格按钮 */
QPushButton {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary_light}, stop:1 {primary});
    border: 2px solid {border};
    border-radius: 4px;
    color: {text_primary};
    padding: 10px 18px;
    font-weight: 600;
    font-size: 13px;
    min-width: 90px;
    min-height: 36px;
    text-transform: uppercase;
    letter-spacing: 1px;
}}

QPushButton:hover {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary}, stop:1 {primary_dark});
    border-color: {text_secondary};
    box-shadow: 0 0 8px rgba(195, 176, 145, 0.3);
}}

QPushButton:pressed {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary_dark}, stop:1 rgba(42, 53, 42, 0.9));
    border-color: {warning};
    box-shadow: inset 0 0 6px rgba(255, 107, 53, 0.4);
}}

/* 战术行动按钮 */
QPushButton[class="tactical"] {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {warning}, stop:1 rgba(229, 90, 43, 0.9));
    border: 2px solid #FF7F50;
    color: #FFFFFF;
    font-weight: 700;
    text-shadow: 0 1px 2px rgba(0,0,0,0.7);
}}

QPushButton[class="tactical"]:hover {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FF7F50, stop:1 {warning});
    box-shadow: 0 0 12px rgba(255, 107, 53, 0.6);
}}

/* HUD风格按钮 */
QPushButton[class="hud"] {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {tactical}, stop:1 rgba(39, 174, 96, 0.9));
    border: 1px solid {tactical};
    color: #FFFFFF;
    border-radius: 8px;
    font-family: "Consolas", monospace;
}}

/* 输入框 - HUD风格 */
QLineEdit, QTextEdit, QPlainTextEdit {{
    border: 2px solid {border};
    border-radius: 4px;
    padding: 10px 14px;
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {surface}, stop:1 rgba(44, 44, 44, 0.9));
    selection-background-color: {primary};
    color: {text_primary};
    font-family: "Consolas", monospace;
}}

QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {{
    border-color: {tactical};
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {background}, stop:1 rgba(44, 44, 44, 0.95));
    box-shadow: 0 0 6px rgba(46, 204, 113, 0.4);
}}

/* 下拉框 */
QComboBox {{
    border: 2px solid {border};
    border-radius: 4px;
    padding: 10px 14px;
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {surface}, stop:1 rgba(44, 44, 44, 0.9));
    color: {text_primary};
    min-width: 100px;
    font-family: "Consolas", monospace;
}}

QComboBox:focus {{
    border-color: {tactical};
}}

/* 滑块 - 雷达扫描风格 */
QSlider::groove:horizontal {{
    border: 1px solid {border};
    height: 10px;
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 {background}, stop:0.5 rgba(44, 44, 44, 0.8), stop:1 {background});
    border-radius: 5px;
}}

QSlider::handle:horizontal {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {tactical}, stop:1 rgba(39, 174, 96, 0.9));
    border: 2px solid {text_primary};
    width: 22px;
    margin: -8px 0;
    border-radius: 11px;
    box-shadow: 0 0 8px rgba(46, 204, 113, 0.6);
}}

/* 进度条 - HUD风格 */
QProgressBar {{
    border: 2px solid {border};
    border-radius: 6px;
    text-align: center;
    font-weight: 600;
    font-family: "Consolas", monospace;
    background-color: {surface};
    color: {text_primary};
    height: 24px;
}}

QProgressBar::chunk {{
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 {tactical}, stop:0.5 {secondary}, stop:1 {tactical});
    border-radius: 4px;
}}

/* 表格 - 数据可视化风格 */
QTableWidget, QTableView {{
    gridline-color: {border};
    background-color: {surface};
    alternate-background-color: rgba(44, 44, 44, 0.9);
    border: 2px solid {border};
    border-radius: 6px;
    selection-background-color: {highlight};
}}

QTableWidget::item, QTableView::item {{
    padding: 10px;
    border-bottom: 1px solid rgba(139, 115, 85, 0.3);
    color: {text_primary};
    font-family: "Consolas", monospace;
}}

QTableWidget::item:selected, QTableView::item:selected {{
    background-color: {highlight};
    color: #FFFFFF;
    border: 1px solid {tactical};
}}

QHeaderView::section {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary}, stop:1 {primary_dark});
    padding: 10px 14px;
    border: none;
    border-right: 1px solid {border};
    border-bottom: 2px solid {text_secondary};
    font-weight: 700;
    font-family: "Consolas", monospace;
    color: {text_primary};
    text-transform: uppercase;
    letter-spacing: 1px;
}}

/* 分组框 - 战术设备风格 */
QGroupBox {{
    font-weight: 700;
    border: 3px solid {border};
    border-radius: 8px;
    margin-top: 12px;
    padding-top: 20px;
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {surface}, stop:1 rgba(44, 44, 44, 0.9));
    color: {text_primary};
    font-family: "Consolas", monospace;
}}

QGroupBox::title {{
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 12px 0 12px;
    background: {primary};
    border: 2px solid {text_secondary};
    border-radius: 4px;
    color: {text_primary};
    text-transform: uppercase;
    letter-spacing: 1px;
}}

/* 选项卡 - 控制面板风格 */
QTabBar::tab {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary_dark}, stop:1 rgba(42, 53, 42, 0.9));
    border: 2px solid {border};
    padding: 12px 18px;
    margin-right: 2px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    color: {text_secondary};
    font-weight: 600;
    font-family: "Consolas", monospace;
    text-transform: uppercase;
    letter-spacing: 1px;
}}

QTabBar::tab:selected {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {primary}, stop:1 {primary_dark});
    border-bottom: 3px solid {tactical};
    color: {text_primary};
}}

/* 菜单 - 指挥界面风格 */
QMenuBar {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(44, 44, 44, 0.8), stop:1 {background});
    border-bottom: 2px solid {border};
}}

QMenuBar::item:selected {{
    background-color: {primary};
    color: #FFFFFF;
    border: 1px solid {tactical};
}}

QMenu::item:selected {{
    background-color: {primary};
    color: #FFFFFF;
    border: 1px solid {tactical};
}}

/* 工具提示 - HUD显示风格 */
QToolTip {{
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 {highlight}, stop:1 rgba(61, 74, 61, 0.9));
    color: {text_primary};
    border: 2px solid {tactical};
    border-radius: 6px;
    padding: 8px 12px;
    font-size: 12px;
    font-family: "Consolas", monospace;
    font-weight: 600;
    box-shadow: 0 0 8px rgba(46, 204, 113, 0.6);
}}

/* 滚动条 */
QScrollBar:vertical {{
    background-color: {surface};
    width: 14px;
    border-radius: 7px;
    border: 1px solid {border};
}}

QScrollBar::handle:vertical {{
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 {primary}, stop:1 {primary_dark});
    border-radius: 7px;
    min-height: 25px;
    border: 1px solid {text_secondary};
}}

QScrollBar::handle:vertical:hover {{
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 {primary_light}, stop:1 {primary});
    border: 1px solid {tactical};
}}
"""

    def _get_current_time(self) -> str:
        """获取当前时间"""
        from datetime import datetime
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    def generate_from_config(self, config_file: str, output_dir: str) -> List[str]:
        """从配置文件生成主题"""
        try:
            with open(config_file, 'r', encoding='utf-8') as f:
                config = json.load(f)

            generated_files = []

            for theme_config in config.get('themes', []):
                name = theme_config['name']
                colors = theme_config['colors']
                style = theme_config.get('style', 'modern')

                output_path = os.path.join(output_dir, f"{name.lower().replace(' ', '-')}.qss")
                self.generate_theme(name, colors, style, output_path)
                generated_files.append(output_path)

            return generated_files

        except Exception as e:
            print(f"❌ 配置文件处理失败: {e}")
            return []

def main():
    parser = argparse.ArgumentParser(description='Qt主题生成器')
    parser.add_argument('--name', required=True, help='主题名称')
    parser.add_argument('--primary-color', default='#2196F3', help='主色调 (默认: #2196F3)')
    parser.add_argument('--secondary-color', default='#FF5722', help='辅助色 (默认: #FF5722)')
    parser.add_argument('--background', default='#FAFAFA', help='背景色 (默认: #FAFAFA)')
    parser.add_argument('--text-color', default='#37474F', help='文字颜色 (默认: #37474F)')
    parser.add_argument('--style', choices=['modern', 'minimal', 'military'], default='modern', help='风格类型 (默认: modern)')
    parser.add_argument('--output', default='./theme.qss', help='输出文件路径 (默认: ./theme.qss)')
    parser.add_argument('--scheme', choices=['blue', 'green', 'purple', 'dark', 'military'], help='使用预定义颜色方案')
    parser.add_argument('--config', help='从配置文件批量生成主题')

    args = parser.parse_args()

    generator = ThemeGenerator()

    if args.config:
        # 批量生成模式
        output_dir = os.path.dirname(args.output) or '.'
        files = generator.generate_from_config(args.config, output_dir)
        print(f"✅ 批量生成完成，共生成 {len(files)} 个主题")
        return

    # 获取颜色配置
    if args.scheme:
        colors = generator.color_schemes[args.scheme].copy()
        # 使用命令行参数覆盖颜色方案
        if args.primary_color != '#2196F3':
            colors['primary'] = args.primary_color
            colors['primary_light'] = args.primary_color
            colors['primary_dark'] = args.primary_color
        if args.secondary_color != '#FF5722':
            colors['accent'] = args.secondary_color
    else:
        colors = {
            'primary': args.primary_color,
            'primary_light': args.primary_color,
            'primary_dark': args.primary_color,
            'accent': args.secondary_color,
            'background': args.background,
            'surface': '#FFFFFF',
            'text_primary': args.text_color,
            'text_secondary': '#616161',
            'border': '#E0E0E0',
            'shadow': 'rgba(0,0,0,0.12)'
        }

    # 生成主题
    generator.generate_theme(args.name, colors, args.style, args.output)
    print(f"✅ 主题 '{args.name}' 生成完成！")

if __name__ == '__main__':
    main()