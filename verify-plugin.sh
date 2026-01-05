#!/bin/bash

# MyClaudeSkills 插件验证脚本

echo "======================================"
echo "MyClaudeSkills 插件验证"
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
        echo -e "${GREEN}✓${NC} $1 存在"
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
        echo -e "${GREEN}✓${NC} $1 存在"
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

echo "1. 检查插件配置文件"
echo "-------------------"
check_file ".claude-plugin/plugin.json"
check_json ".claude-plugin/plugin.json"
check_file ".claude-plugin/marketplace.json"
check_json ".claude-plugin/marketplace.json"
echo ""

echo "2. 检查文档文件"
echo "-------------------"
check_file "README.md"
check_file "INSTALL.md"
check_file "CHANGELOG.md"
check_file "CLAUDE.md"
echo ""

echo "3. 检查技能目录"
echo "-------------------"
check_dir "skills"
check_dir "skills/qt-compiler-errors"
check_dir "skills/qt-cpp-review"
check_dir "skills/qt-ui-optimization"
check_dir "skills/gitea"
echo ""

echo "4. 检查技能文件"
echo "-------------------"
check_file "skills/qt-compiler-errors/skill.md"
check_file "skills/qt-cpp-review/SKILL.md"
check_file "skills/qt-ui-optimization/SKILL.md"
check_file "skills/gitea/SKILL.md"
echo ""

echo "5. 验证技能 front matter"
echo "-------------------"
for skill_file in skills/qt-compiler-errors/skill.md skills/qt-cpp-review/SKILL.md skills/qt-ui-optimization/SKILL.md skills/gitea/SKILL.md; do
    if grep -q "^---" "$skill_file" && grep -q "^name:" "$skill_file" && grep -q "^description:" "$skill_file"; then
        echo -e "${GREEN}✓${NC} $skill_file front matter 正确"
        ((passed++))
    else
        echo -e "${RED}✗${NC} $skill_file front matter 不完整"
        ((failed++))
    fi
done
echo ""

echo "6. 检查插件元数据"
echo "-------------------"
if grep -q '"name": "myclaudeskills"' .claude-plugin/plugin.json; then
    echo -e "${GREEN}✓${NC} 插件名称正确"
    ((passed++))
else
    echo -e "${RED}✗${NC} 插件名称不正确"
    ((failed++))
fi

if grep -q '"version":' .claude-plugin/plugin.json; then
    echo -e "${GREEN}✓${NC} 插件版本已定义"
    ((passed++))
else
    echo -e "${RED}✗${NC} 插件版本未定义"
    ((failed++))
fi
echo ""

echo "======================================"
echo "验证结果"
echo "======================================"
echo -e "${GREEN}通过: $passed${NC}"
echo -e "${RED}失败: $failed${NC}"
echo ""

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}✓ 插件配置验证通过！${NC}"
    echo ""
    echo "下一步："
    echo "1. 在 Claude Code 中运行: /plugin marketplace add $(pwd)"
    echo "2. 然后运行: /plugin install myclaudeskills@myclaudeskills-marketplace"
    echo "3. 重启 Claude Code"
    exit 0
else
    echo -e "${RED}✗ 插件配置存在问题，请检查上述错误${NC}"
    exit 1
fi
