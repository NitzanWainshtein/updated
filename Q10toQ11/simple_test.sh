#!/bin/bash

# Simple Test - Only Required Tools
echo "🧪 Q10-Q11 Basic Requirements Test"
echo "=================================="

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

errors=0

echo -e "\n🔧 Checking Required Tools"
echo "---------------------------"

# Only check what's actually needed
required_tools=("gcc" "g++" "make" "valgrind" "gcov")
for tool in "${required_tools[@]}"; do
    if command -v "$tool" >/dev/null 2>&1; then
        echo -e "✅ $tool: ${GREEN}Found${NC}"
    else
        echo -e "❌ $tool: ${RED}Missing${NC}"
        errors=$((errors + 1))
    fi
done

echo -e "\n📁 Checking Projects"
echo "--------------------"

projects=("Q1toQ4" "Q6" "Q7" "Q8" "Q9")
for project in "${projects[@]}"; do
    if [ -d "../$project" ] && [ -f "../$project/Makefile" ]; then
        echo -e "✅ $project: ${GREEN}Ready${NC}"
    else
        echo -e "❌ $project: ${RED}Missing${NC}"
        errors=$((errors + 1))
    fi
done

echo -e "\n🎯 Final Check"
echo "--------------"

if [ $errors -eq 0 ]; then
    echo -e "${GREEN}🎉 All basic requirements met!${NC}"
    echo ""
    echo "Run: make all"
    exit 0
else
    echo -e "${RED}❌ Missing requirements: $errors${NC}"
    echo ""
    echo "Install: sudo apt-get install valgrind"
    exit 1
fi