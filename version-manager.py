#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MyClaudeSkills 版本管理脚本
实现项目记忆规则和版本控制逻辑
"""

import re
import json
import subprocess
from datetime import datetime
from typing import Tuple, Optional

class VersionManager:
    def __init__(self):
        self.package_json_path = "package.json"
        self.changelog_path = "CHANGELOG.md"
        self.current_version = self._get_current_version()

    def _get_current_version(self) -> str:
        """获取当前版本号"""
        try:
            with open(self.package_json_path, 'r', encoding='utf-8') as f:
                package_data = json.load(f)
                return package_data.get('version', '0.0.1')
        except FileNotFoundError:
            return '0.0.1'

    def _parse_version(self, version: str) -> Tuple[int, int, int]:
        """解析版本号为 (主版本, 次版本, 修订号)"""
        match = re.match(r'(\d+)\.(\d+)\.(\d+)', version)
        if match:
            return tuple(map(int, match.groups()))
        return (0, 0, 1)

    def _format_version(self, major: int, minor: int, patch: int) -> str:
        """格式化版本号"""
        return f"{major}.{minor}.{patch}"

    def _update_package_json(self, new_version: str):
        """更新package.json中的版本号"""
        try:
            with open(self.package_json_path, 'r', encoding='utf-8') as f:
                package_data = json.load(f)

            package_data['version'] = new_version
            package_data['config']['lastUpdated'] = datetime.now().strftime('%Y-%m-%d')

            with open(self.package_json_path, 'w', encoding='utf-8') as f:
                json.dump(package_data, f, indent=2, ensure_ascii=False)

            print(f"✓ package.json版本已更新至 {new_version}")
        except Exception as e:
            print(f"✗ 更新package.json失败: {e}")

    def _add_changelog_entry(self, new_version: str, changes: str, change_type: str = "修改"):
        """在CHANGELOG.md中添加新版本条目"""
        try:
            with open(self.changelog_path, 'r', encoding='utf-8') as f:
                content = f.read()

            today = datetime.now().strftime('%Y-%m-%d')
            new_entry = f"## [{new_version}] - {today}\n\n### {change_type}\n{changes}\n\n### 项目信息\n- 版本：{new_version}\n- 维护者：mm-kzl\n- 邮箱：mm-kzl@outlook.com\n- 更新时间：{today}\n- 项目类型：Claude技能库（git子模块）\n\n---\n\n"

            # 在第一个版本条目之前插入新条目
            pattern = r'(## \[\d+\.\d+\.\d+\] - \d{4}-\d{2}-\d{2})'
            if re.search(pattern, content):
                content = re.sub(pattern, new_entry + r'\1', content, count=1)
            else:
                content += new_entry

            with open(self.changelog_path, 'w', encoding='utf-8') as f:
                f.write(content)

            print(f"✓ CHANGELOG.md已添加版本 {new_version} 条目")
        except Exception as e:
            print(f"✗ 更新CHANGELOG.md失败: {e}")

    def add_submodule(self, submodule_name: str, submodule_version: str = "v1.0.0"):
        """新增子模块 - 次版本号+1，修订号重置为0"""
        major, minor, patch = self._parse_version(self.current_version)
        new_minor = minor + 1
        new_version = self._format_version(major, new_minor, 0)

        changes = f"- 新增子模块：{submodule_name}（版本：{submodule_version}）\n- 次版本号递增：v{self.current_version} → v{new_version}"

        self._update_package_json(new_version)
        self._add_changelog_entry(new_version, changes, "新增")

        print(f"✅ 子模块添加完成，版本更新至 v{new_version}")
        return new_version

    def update_submodule(self, submodule_name: str, old_version: str, new_version: str):
        """子模块更新 - 修订号+1"""
        major, minor, patch = self._parse_version(self.current_version)
        new_patch = patch + 1
        new_version_formatted = self._format_version(major, minor, new_patch)

        changes = f"- 子模块更新：{submodule_name}（{old_version} → {new_version}）\n- 修订号递增：v{self.current_version} → v{new_version_formatted}"

        self._update_package_json(new_version_formatted)
        self._add_changelog_entry(new_version_formatted, changes, "修改")

        print(f"✅ 子模块更新完成，版本更新至 v{new_version_formatted}")
        return new_version_formatted

    def major_version_update(self, new_major: int, description: str):
        """主版本更新 - 需要创建归档分支"""
        major, minor, patch = self._parse_version(self.current_version)
        new_version = self._format_version(new_major, 0, 0)
        archive_branch = f"v{major}.x.x-archive"

        changes = f"- 主版本重大更新：{description}\n- 版本跳跃：v{self.current_version} → v{new_version}\n- 创建归档分支：{archive_branch}\n- 主线继续开发"

        # 创建归档分支的git命令
        try:
            subprocess.run(['git', 'checkout', '-b', archive_branch], check=True, capture_output=True)
            print(f"✓ 已创建归档分支 {archive_branch}")
            subprocess.run(['git', 'checkout', 'main'], check=True, capture_output=True)
            print("✓ 已切换回main分支")
        except subprocess.CalledProcessError as e:
            print(f"⚠ 创建git分支失败，请手动处理: {e}")

        self._update_package_json(new_version)
        self._add_changelog_entry(new_version, changes, "重大更新")

        print(f"✅ 主版本更新完成，版本更新至 v{new_version}")
        return new_version

    def get_current_status(self):
        """获取当前版本状态"""
        major, minor, patch = self._parse_version(self.current_version)
        print(f"📋 当前版本状态:")
        print(f"   版本号: v{self.current_version}")
        print(f"   主版本: {major}")
        print(f"   次版本: {minor}")
        print(f"   修订号: {patch}")
        print(f"   下次新增子模块: v{self._format_version(major, minor + 1, 0)}")
        print(f"   下次子模块更新: v{self._format_version(major, minor, patch + 1)}")

if __name__ == "__main__":
    import sys

    vm = VersionManager()

    if len(sys.argv) < 2:
        print("MyClaudeSkills 版本管理工具")
        print("使用方法:")
        print("  python version-manager.py status                    # 查看当前版本状态")
        print("  python version-manager.py add <子模块名> [版本]      # 新增子模块")
        print("  python version-manager.py update <子模块名> <旧版> <新版> # 更新子模块")
        print("  python version-manager.py major <新主版本> <描述>    # 主版本更新")
        sys.exit(1)

    command = sys.argv[1]

    if command == "status":
        vm.get_current_status()
    elif command == "add" and len(sys.argv) >= 3:
        submodule_name = sys.argv[2]
        submodule_version = sys.argv[3] if len(sys.argv) > 3 else "v1.0.0"
        vm.add_submodule(submodule_name, submodule_version)
    elif command == "update" and len(sys.argv) >= 5:
        submodule_name = sys.argv[2]
        old_version = sys.argv[3]
        new_version = sys.argv[4]
        vm.update_submodule(submodule_name, old_version, new_version)
    elif command == "major" and len(sys.argv) >= 4:
        new_major = int(sys.argv[2])
        description = sys.argv[3]
        vm.major_version_update(new_major, description)
    else:
        print("❌ 参数错误，请检查使用方法")
        sys.exit(1)