#!/bin/sh
echo "setup vundle..."
 
mkdir -p $HOME/.vim/plugins
VUNDLE_DIR="$HOME/.vim/plugins/Vundle.vim"
rm -rf $VUNDLE_DIR
git clone https://github.com/VundleVim/Vundle.vim.git $VUNDLE_DIR

echo "download vimrc config..."
curl https://raw.githubusercontent.com/Flynston/ProgMonk/master/Arch/i3wm/laptop_config_files/vimrc > $HOME/.vimrc

echo "setup color scheme config..."
mkdir -p $HOME/.vim/colors
curl https://raw.githubusercontent.com/Flynston/ProgMonk/master/Arch/i3wm/laptop_config_files/vim/colors/vendetta.vim > $HOME/.vim/colors/vendetta.vim

echo "install Vim plugins..."
vim +PluginInstall +qall
