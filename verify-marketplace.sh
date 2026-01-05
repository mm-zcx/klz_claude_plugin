#!/bin/bash

# MyClaudeSkills 插件市场验证脚本

echo "======================================"
echo "MyClaudeSkills 插件市场验证"
echo "======================================"
echo ""

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 检查计数
passed=0
failed=0

# 检查函数
check_file() {
    if [ -f "$1" ]; then
        echo -e "${GREEN}✓${NC} $1"
        ((passed++))
        return 0
    else
        echo -e "${RED}✗${NC} $1 不存在"
        ((failed++))
        return 1
    fi
}

check_dir() {
    if [ -d "$1" ]; then
        echo -e "${GREEN}✓${NC} $1"
        ((passed++))
        return 0
    else
        echo -e "${RED}✗${NC} $1 不存在"
        ((failed++))
        return 1
    fi
}

check_json() {
    if python3 -m json.tool "$1" > /dev/null 2>&1; then
        echo -e "${GREEN}✓${NC} $1 JSON 格式正确"
        ((passed++))
        return 0
    else
        echo -e "${RED}✗${NC} $1 JSON 格式错误"
        ((failed++))
        return 1
    fi
}

echo "1. 检查市场配置"
echo "-------------------"
check_file ".claude-plugin/marketplace.json"
check_json ".claude-plugin/marketplace.json"
echo ""

echo "2. 检查插件目录"
echo "-------------------"
check_dir "plugins"
check_dir "plugins/qt-compiler-errors"
check_dir "plugins/qt-cpp-review"
check_dir "plugins/qt-ui-optimization"
check_dir "plugins/gitea"
echo ""

echo "3. 检查插件配置文件"
echo "-------------------"
check_file "plugins/qt-compiler-errors/.claude-plugin/plugin.json"
check_json "plugins/qt-compiler-errors/.claude-plugin/plugin.json"
check_file "plugins/qt-cpp-review/.claude-plugin/plugin.json"
check_json "plugins/qt-cpp-review/.claude-plugin/plugin.json"
check_file "plugins/qt-ui-optimization/.claude-plugin/plugin.json"
check_json "plugins/qt-ui-optimization/.claude-plugin/plugin.json"
check_file "plugins/gitea/.claude-plugin/plugin.json"
check_json "plugins/gitea/.claude-plugin/plugin.json"
echo ""

echo "4. 检查技能文件"
echo "-------------------"
check_file "plugins/qt-compiler-errors/skill.md"
check_file "plugins/qt-cpp-review/SKILL.md"
check_file "plugins/qt-ui-optimization/SKILL.md"
check_file "plugins/gitea/SKILL.md"
echo ""

echo "5. 验证技能 front matter"
echo "-------------------"
for skill_file in plugins/qt-compiler-errors/skill.md plugins/qt-cpp-review/SKILL.md plugins/qt-ui-optimization/SKILL.md plugins/gitea/SKILL.md; do
    if grep -q "^---" "$skill_file" && grep -q "^name:" "$skill_file" && grep -q "^description:" "$skill_file"; then
        echo -e "${GREEN}✓${NC} $skill_file front matter 正确"
        ((passed++))
    else
        echo -e "${RED}✗${NC} $skill_file front matter 不完整"
        ((failed++))
    fi
done
echo ""

echo "6. 验证插件元数据"
echo "-------------------"
for plugin_dir in plugins/*/; do
    plugin_name=$(basename "$plugin_dir")
    plugin_json="$plugin_dir.claude-plugin/plugin.json"

    if [ -f "$plugin_json" ]; then
        if grep -q "\"name\": \"$plugin_name\"" "$plugin_json"; then
            echo -e "${GREEN}✓${NC} $plugin_name 插件名称正确"
            ((passed++))
        else
            echo -e "${RED}✗${NC} $plugin_name 插件名称不匹配"
            ((failed++))
        fi

        if grep -q "\"version\":" "$plugin_json"; then
            version=$(grep "\"version\":" "$plugin_json" | sed 's/.*"version": *"\([^"]*\)".*/\1/')
            echo -e "${GREEN}✓${NC} $plugin_name 版本: $version"
            ((passed++))
        else
            echo -e "${RED}✗${NC} $plugin_name 版本未定义"
            ((failed++))
        fi
    fi
done
echo ""

echo "======================================"
echo "验证结果"
echo "======================================"
echo -e "${GREEN}通过: $passed${NC}"
echo -e "${RED}失败: $failed${NC}"
echo ""

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}✓ 插件市场配置验证通过！${NC}"
    echo ""
    echo "下一步："
    echo "1. 在 Claude Code 中运行: /plugin marketplace add $(pwd)"
    echo "2. 浏览可用插件: /plugin"
    echo "3. 安装单个插件，例如: /plugin install qt-compiler-errors@myclaudeskills-marketplace"
    exit 0
else
    echo -e "${RED}✗ 插件市场配置存在问题，请检查上述错误${NC}"
    exit 1
fi
