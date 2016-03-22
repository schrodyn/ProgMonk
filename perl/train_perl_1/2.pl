#!/usr/bin/env perl


#print @ARGV;

$\ = $/;
$, = ",";



=cut
for $c (`iptables -L`){
	chomp $c;
	print "|$c|\n" if $c;
}
=cut

$a = "ананас";
print $a=~/ана/g;

=cut

__END__
$c="192.168.1.27";
print $c=~/(\d+)\.\d+\.\d+\.(\d+)/;

print "$1 lol $2";

__END__
$c = "foo";
print $c=~/foo|bar/;

=cut

$cmd = shift @ARGV;

if ($cmd eq 'block'){
	for (@ARGV){
		`iptables -A INPUT -s $_ -j DROP`;
	}
}
elsif ($cmd eq 'list'){
	for $c (`iptables -L`){
		if($c=~/FORWARD(.+)/){
			print $1;
		}
	}
}

__END__
$\ = $/;
$, = ",";

$a=`iptables -L`;
$a=system('iptables -L');

print $a
