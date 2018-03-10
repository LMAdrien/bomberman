# Basic vagrantfile to devel C program at ETNA School
# To Do automation setup header login emacs, vim

Vagrant.configure(2) do |config|

 config.vm.box = "puppetlabs/debian-8.2-64-puppet"
 # Link update to Debian 8
 #config.vm.box_url  = "https://atlas.hashicorp.com/puppetlabs/debian-8.2-64-puppet"
 config.vm.hostname = "etna-devel-c"
 config.vm.network "public_network"
 # Working synced directory
 config.vm.synced_folder "./", "/home/vagrant/devel"
 
 # Memory 
 config.vm.provider "virtualbox" do |vb|
  vb.memory = "2048"
  vb.gui = true
  vb.cpus = 4
 end
  
$script = <<SCRIPT
  apt-get update
  apt-get install -y gcc make vim emacs libsdl2-image-2.0-0 libsdl2-2.0-0 libsdl2-dev libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev gnome
SCRIPT

  config.vm.provision "shell", inline: $script
end
