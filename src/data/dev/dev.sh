#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Put the following lines at the bottom of $HOME/.bashrc:"
echo "Make sure to replace /path/to/neper (probably by $(echo $PWD | sed -e "s/\/src\/build//g")?)"
echo "-------------------------------------------------------------------"
cat $SCRIPT_DIR/bashrc
echo "-------------------------------------------------------------------"

echo ""
echo "Put the following lines in your $HOME/.tmux.conf:"
echo "-------------------------------------------------------------------"
cat $SCRIPT_DIR/tmux.conf
echo "-------------------------------------------------------------------"

echo ""
echo "Put and complete the following lines in your $HOME/.gitconfig:"
echo "-------------------------------------------------------------------"
cat $SCRIPT_DIR/gitconfig
echo "-------------------------------------------------------------------"
echo ""

echo "Copying Neper's custom development commands to ~/.local/bin/nepertools..."
mkdir -p ~/.local/bin/nepertools
cp "$SCRIPT_DIR"/* ~/.local/bin/nepertools
rm -f ~/.local/bin/nepertools/dev.sh ~/.local/bin/nepertools/bashrc

exit 0
