#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo ""
echo -e "\033[1mPut the following lines at the bottom of $HOME/.bashrc:\033[0m"
echo -e "\033[1mMake sure to replace /path/to/neper (probably by $(echo $PWD | sed 's/\(.*neper\).*/\1/'))\033[0m"
echo "-------------------------------------------------------------------"
cat $SCRIPT_DIR/bashrc
echo "-------------------------------------------------------------------"

echo ""
echo -e "\033[1mPut the following lines in your $HOME/.tmux.conf:\033[0m"
echo "-------------------------------------------------------------------"
cat $SCRIPT_DIR/tmux.conf
echo "-------------------------------------------------------------------"

echo ""
echo -e "\033[1mPut and complete the following lines in your $HOME/.gitconfig:\033[0m"
echo "-------------------------------------------------------------------"
cat $SCRIPT_DIR/gitconfig
echo "-------------------------------------------------------------------"
echo ""

echo -e "\033[1mCopying Neper's custom development commands to ~/.local/bin/nepertools...\033[0m"
mkdir -p ~/.local/bin/nepertools
cp "$SCRIPT_DIR"/* ~/.local/bin/nepertools
ln -fs ~/.local/bin/nepertools/neperfepx-asy ~/.local/bin/nepertools/nasy
for file in dev.sh bashrc pens*asy
do
  rm -f ~/.local/bin/nepertools/$file
done

echo -e "\033[1mCopying Neper's Asymptote shared files to ~/.asy...\033[0m"
mkdir -p ~/.asy
cp "$SCRIPT_DIR"/*.asy ~/.asy

exit 0
