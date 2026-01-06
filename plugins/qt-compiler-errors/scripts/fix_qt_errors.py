#!/usr/bin/env python3
"""
Qt编译错误诊断和修复脚本
自动检测并修复常见的Qt编译问题
"""

import os
import re
import subprocess
import sys
from pathlib import Path

class QtErrorFixer:
    def __init__(self, project_path="."):
        self.project_path = Path(project_path)
        self.fixes_applied = []

    def scan_and_fix(self):
        """扫描并修复Qt编译错误"""
        print("🔍 扫描Qt项目中的常见编译错误...")

        # 扫描各种文件类型
        self.fix_header_files()
        self.fix_source_files()
        self.fix_cmake_files()
        self.fix_pro_files()

        if self.fixes_applied:
            print(f"\n✅ 应用了 {len(self.fixes_applied)} 个修复")
            print("建议重新构建项目:")
            print("  make clean && qmake && make")
        else:
            print("\n✅ 未发现需要修复的常见错误")

    def fix_header_files(self):
        """修复头文件中的常见错误"""
        header_files = list(self.project_path.glob("**/*.h")) + list(self.project_path.glob("**/*.hpp"))

        for header_file in header_files:
            self._fix_header_file(header_file)

    def _fix_header_file(self, file_path):
        """修复单个头文件"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()

            original_content = content
            modified = False

            # 修复1: 检查是否缺少Q_OBJECT宏
            if self._needs_q_object(content):
                content = self._add_q_object_macro(content)
                modified = True
                self.fixes_applied.append(f"添加Q_OBJECT宏: {file_path}")

            # 修复2: 检查头文件保护
            if not self._has_include_guard(content):
                content = self._add_include_guard(content, file_path.stem)
                modified = True
                self.fixes_applied.append(f"添加头文件保护: {file_path}")

            if modified and content != original_content:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"🔧 修复了头文件: {file_path}")

        except Exception as e:
            print(f"⚠️  处理头文件时出错 {file_path}: {e}")

    def fix_source_files(self):
        """修复源文件中的常见错误"""
        source_files = list(self.project_path.glob("**/*.cpp")) + list(self.project_path.glob("**/*.cc"))

        for source_file in source_files:
            self._fix_source_file(source_file)

    def _fix_source_file(self, file_path):
        """修复单个源文件"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()

            original_content = content
            modified = False

            # 修复1: 替换过时的Qt信号
            if "QAbstractSocket::error" in content:
                content = content.replace(
                    "QAbstractSocket::error",
                    "QAbstractSocket::errorOccurred"
                )
                modified = True
                self.fixes_applied.append(f"更新Qt信号名: {file_path}")

            # 修复2: 添加Q_UNUSED宏到未使用参数
            content = self._add_q_unused_macros(content)
            if content != original_content:
                modified = True

            # 修复3: 移除手动的.moc包含
            if "#include \"" in content and ".moc\"" in content:
                content = re.sub(r'#include\s+"[^"]*\.moc"[^\\n]*',
                               '// MOC文件由Qt自动生成', content)
                modified = True
                self.fixes_applied.append(f"移除手动.moc包含: {file_path}")

            if modified:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"🔧 修复了源文件: {file_path}")

        except Exception as e:
            print(f"⚠️  处理源文件时出错 {file_path}: {e}")

    def fix_cmake_files(self):
        """修复CMakeLists.txt文件"""
        cmake_files = list(self.project_path.glob("**/CMakeLists.txt"))

        for cmake_file in cmake_files:
            self._fix_cmake_file(cmake_file)

    def _fix_cmake_file(self, file_path):
        """修复单个CMakeLists.txt文件"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()

            original_content = content
            modified = False

            # 检查并添加Qt5 Network模块
            if "Qt5::Core" in content and "Qt5::Network" not in content:
                lines = content.split('\n')
                new_lines = []
                i = 0

                while i < len(lines):
                    line = lines[i]
                    new_lines.append(line)

                    # 在find_package后添加Network模块
                    if "find_package(Qt5" in line and "Qt5::Network" not in content:
                        # 找到REQUIRED COMPONENTS行或在该行后添加
                        if "COMPONENTS" in line and i+1 < len(lines):
                            new_lines[i] = line.replace(")", " Network)")
                            modified = True
                            self.fixes_applied.append(f"添加Qt5::Network模块: {file_path}")
                    i += 1

                content = '\n'.join(new_lines)

            # 检查target_link_libraries部分
            if "Qt5::Core" in content and "Qt5::Network" not in content:
                lines = content.split('\n')
                for i, line in enumerate(lines):
                    if "target_link_libraries" in line and "Qt5::Core" in line:
                        lines[i] = line.replace("Qt5::Core", "Qt5::Core Qt5::Network")
                        modified = True
                        self.fixes_applied.append(f"添加Qt5::Network链接: {file_path}")
                        break
                content = '\n'.join(lines)

            if modified:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"🔧 修复了CMake文件: {file_path}")

        except Exception as e:
            print(f"⚠️  处理CMake文件时出错 {file_path}: {e}")

    def fix_pro_files(self):
        """修复.pro文件"""
        pro_files = list(self.project_path.glob("**/*.pro"))

        for pro_file in pro_files:
            self._fix_pro_file(pro_file)

    def _fix_pro_file(self, file_path):
        """修复单个.pro文件"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()

            original_content = content
            modified = False

            # 检查并添加network模块
            if "QT +=" in content and "network" not in content.lower():
                lines = content.split('\n')
                for i, line in enumerate(lines):
                    if line.strip().startswith("QT +=") and "network" not in line.lower():
                        lines[i] = line.rstrip() + " network"
                        modified = True
                        self.fixes_applied.append(f"添加Qt network模块: {file_path}")
                        break
                content = '\n'.join(lines)

            if modified:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"🔧 修复了.pro文件: {file_path}")

        except Exception as e:
            print(f"⚠️  处理.pro文件时出错 {file_path}: {e}")

    def _needs_q_object(self, content):
        """检查是否需要Q_OBJECT宏"""
        # 简单检查：如果有signals/slots但没有Q_OBJECT
        has_signals_slots = "signals:" in content or "slots:" in content
        has_q_object = "Q_OBJECT" in content
        inherits_qobject = "QObject" in content

        return has_signals_slots and not has_q_object and inherits_qobject

    def _add_q_object_macro(self, content):
        """添加Q_OBJECT宏到类定义中"""
        lines = content.split('\n')
        new_lines = []

        for i, line in enumerate(lines):
            new_lines.append(line)

            # 在class定义后添加Q_OBJECT
            if re.match(r'\s*class\s+\w+\s*:\s*public\s+QObject', line):
                # 查找下一个非空行
                j = i + 1
                while j < len(lines) and lines[j].strip() == "":
                    new_lines.append(lines[j])
                    j += 1
                new_lines.append("    Q_OBJECT")
                break

        return '\n'.join(new_lines)

    def _has_include_guard(self, content):
        """检查是否有头文件保护"""
        return "#ifndef" in content and "#define" in content and "#endif" in content

    def _add_include_guard(self, content, class_name):
        """添加头文件保护"""
        lines = content.split('\n')
        guard_name = class_name.upper() + "_H"

        new_content = f"""#ifndef {guard_name}
#define {guard_name}

{content}

#endif // {guard_name}"""

        return new_content

    def _add_q_unused_macros(self, content):
        """添加Q_UNUSED宏到未使用的参数"""
        # 简单实现：查找函数参数并添加Q_UNUSED
        # 这是一个简化的实现，实际项目中可能需要更复杂的分析
        lines = content.split('\n')
        new_lines = []

        for line in lines:
            new_lines.append(line)

            # 查找未使用参数的模式
            if "param)" in line and "//" not in line:
                param_match = re.search(r'(\w+)\s+([^&*\s]+)\s+(\w+)\)', line)
                if param_match and param_match.group(3) not in ['void', 'char', 'int', 'float', 'double']:
                    param_name = param_match.group(3)
                    if param_name not in line:
                        # 在函数开始后添加Q_UNUSED
                        next_idx = new_lines.index(line) + 1
                        while next_idx < len(new_lines) and new_lines[next_idx].strip() == "{":
                            new_lines.insert(next_idx + 1, f"    Q_UNUSED({param_name});")
                            break

        return '\n'.join(new_lines)

def main():
    if len(sys.argv) > 1:
        project_path = sys.argv[1]
    else:
        project_path = "."

    fixer = QtErrorFixer(project_path)
    fixer.scan_and_fix()

if __name__ == "__main__":
    main()