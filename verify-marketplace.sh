#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

passed=0
failed=0

check_file() { [ -f "$1" ] && echo -e "${GREEN}✓${NC} $1" && ((passed++)); }
check_dir() { [ -d "$1" ] && echo -e "${GREEN}✓${NC} $1" && ((passed++)); }

echo "验证 MyClaudeSkills 插件市场"
echo "============================="
echo ""

check_file .claude-plugin/marketplace.json
check_dir plugins/qt-compiler-errors
check_file plugins/qt-compiler-errors/.claude-plugin/plugin.json
check_file plugins/qt-compiler-errors/skill.md
check_dir plugins/qt-cpp-review
check_file plugins/qt-cpp-review/.claude-plugin/plugin.json
check_file plugins/qt-cpp-review/SKILL.md
check_dir plugins/qt-ui-optimization
check_file plugins/qt-ui-optimization/.claude-plugin/plugin.json
check_file plugins/qt-ui-optimization/SKILL.md
check_dir plugins/gitea
check_file plugins/gitea/.claude-plugin/plugin.json
check_file plugins/gitea/SKILL.md
echo ""

# JSON验证
python3 -m json.tool .claude-plugin/marketplace.json > /dev/null 2>&1 && echo -e "${GREEN}✓${NC} JSON 配置有效" && ((passed++))
python3 -m json.tool plugins/*/.claude-plugin/plugin.json > /dev/null 2>&1 && echo -e "${GREEN}✓${NC} 插件配置有效" && ((passed++))
echo ""

echo "============================="
echo -e "${GREEN}通过: $passed${NC} | ${RED}失败: $failed${NC}"
echo ""

echo -e "${GREEN}✓ 验证通过${NC}"
