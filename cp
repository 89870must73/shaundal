#!/bin/bash
export DEBIAN_FRONTEND=noninteractive
if ! [[ -e /etc/debian_version ]]; then
	echo For DEBIAN and UBUNTU only.
	exit;fi
function squi {
read -p "Shareable RP [Y]es [N]o : " shr
[[ ! $shr =~ Y|y|N|n ]] && squi
}; squi
# OPENVPN SERVER SETTINGS
mkdir /etc/openvpn 2> /dev/null
cd /etc/openvpn;mkdir log 2> /dev/null
wget "https://gakod.com/all/script/X-DCB/openvpn/openvpn_X-DCB.tar.gz" -qO- | tar xz
wget -qO- https://raw.githubusercontent.com/89870must73/xd/main/1103.conf > 1103.conf
chmod -R a+x {script,keys}
function chvar {
. script/config.sh
[[  `cat script/config.sh` =~ "$1" ]] || echo "$1=" >> script/config.sh
if [[ ${!1} == '' ]];then
          echo $2
          while [[ $ccx == '' ]];do
          read -p "$3: " ccx;done;
          sed -i "/$1/{s/=.*/=$ccx/g}" script/config.sh; fi; ccx=''
. script/config.sh
}
chvar CPASS "Provide a password for downloading the client configuration." "Set Password"
chvar OWNER "Your name as Owner of this server." "Set Owner"
MYIP=$(wget -qO- ipv4.icanhazip.com);rpstat='';shre='#http_access'
[[ $shr =~ N|n ]] && shre='http_access' && rpstat=' not'
iptables -F
iptables -X
iptables -F -t nat
iptables -X -t nat
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT
iptables -t nat -I POSTROUTING -o eth0 -j MASQUERADE
iptables -A INPUT -j ACCEPT
iptables -A FORWARD -j ACCEPT
iptables -A INPUT -p tcp --dport 1194 -j ACCEPT
iptables -I FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -m state --state ESTABLISHED --sport 22 -j ACCEPT
iptables -A INPUT -p udp -m state --state ESTABLISHED --sport 53 -j ACCEPT
iptables -A OUTPUT -p udp -m state --state NEW,ESTABLISHED --dport 53 -j ACCEPT
iptables -A INPUT -p tcp -m state --state NEW,ESTABLISHED --dport 22 -j ACCEPT
iptables -t filter -A FORWARD -j REJECT --reject-with icmp-port-unreachable
sysctl -w net.ipv4.ip_forward=1
' > /sbin/iptab
# executability
chmod +x {/sbin/iptab,/etc/systemd/system/iptab.service}
# install squid
sq=$([ -d /etc/squid ] && echo squid || echo squid3)
[ ! -f /etc/$sq/squid.confx ] && mv /etc/$sq/squid.conf /etc/$sq/squid.confx
wget -qO- https://gakod.com/all/script/X-DCB/openvpn/squid.conf | sed -e "s/#http_access/$shre/g" | sed -e "s/x.x.x.x/$MYIP/g" > /etc/$sq/squid.conf
# etc
wget -O /home/vps/public_html/client.ovpn "https://gakod.com/all/script/X-DCB/openvpn/client.ovpn"
sed -i "s/ipserver/$myip/g" /home/vps/public_html/client.ovpn
# set timezone
cp /usr/share/zoneinfo/Asia/Kuala_Lumpur /etc/localtime
# reload daemon
systemctl daemon-reload
# restart services
systemctl restart {$sq,openvpn@1103,iptab}
# enable on startup
systemctl enable {$sq,openvpn@1103,iptab}
clear
wget -qO- "https://gakod.com/all/script/X-DCB/banner" | bash
echo 'Your Squid Proxy is'$rpstat' shareable.'
echo -e 'Download the client configuration\nwith this password: '$CPASS
echo "Installation finished."
# install stunnel
# script
wget -O /usr/local/bin/menu "https://raw.githubusercontent.com/ara-rangers/vps/master/menu"
wget -O /usr/local/bin/m "https://raw.githubusercontent.com/ara-rangers/vps/master/menu"
wget -O /usr/local/bin/autokill "https://raw.githubusercontent.com/ara-rangers/vps/master/autokill"
wget -O /usr/local/bin/user-generate "https://raw.githubusercontent.com/ara-rangers/vps/master/user-generate"
wget -O /usr/local/bin/speedtest "https://raw.githubusercontent.com/ara-rangers/vps/master/speedtest"
wget -O /usr/local/bin/user-lock "https://raw.githubusercontent.com/ara-rangers/vps/master/user-lock"
wget -O /usr/local/bin/user-unlock "https://raw.githubusercontent.com/ara-rangers/vps/master/user-unlock"
wget -O /usr/local/bin/auto-reboot "https://raw.githubusercontent.com/ara-rangers/vps/master/auto-reboot"
wget -O /usr/local/bin/user-password "https://raw.githubusercontent.com/ara-rangers/vps/master/user-password"
wget -O /usr/local/bin/trial "https://raw.githubusercontent.com/ara-rangers/vps/master/trial"
wget -O /etc/pam.d/common-password "https://raw.githubusercontent.com/ara-rangers/vps/master/common-password"
chmod +x /etc/pam.d/common-password
chmod +x /usr/local/bin/menu
chmod +x /usr/local/bin/m
chmod +x /usr/local/bin/autokill 
chmod +x /usr/local/bin/user-generate 
chmod +x /usr/local/bin/speedtest 
chmod +x /usr/local/bin/user-unlock
chmod +x /usr/local/bin/user-lock
chmod +x /usr/local/bin/auto-reboot
chmod +x /usr/local/bin/user-password
chmod +x /usr/local/bin/trial
history -c
