#!/usr/bin/env perl

$\ = $/;

@space = ([0,1],[-1,0]) x 10;

@a=(qq +A+.. q $z$, q =a= .. qq ;z; );

@a[10..20] = (1..10);

#print "a"+1;
#print "a".1;

#%a = (a,1,b,2,c,3);

$a[a]=a;
print $a[A];
${'\x13\xeb'}=1;

__END__
@a=(1,2,3);

$,=",";
$a[10]=1;

print @a;



__END__
%a = ();
for(A..Z){
	$a{$_}=ord($_);
	print scalar %a;
}

__END__
$a{$_} = ord($_) for (A..K);

$a{d} = 4;

print scalar %a;


__END__
$a{cd} = 5;

print $a{cd};


__END__
$,=",";
%a = (ab=>1,b=>2,c=>3);
$a{ab} = a;


print $a{ab}
#print $a{'a'};

#%a = (1,2,3,4,5,6,7);

#print %a;

__END__
push @a, 123;
pop @a;
shift @a; #like pop but from beginning
unshift @a,123;
#print length(@a);
#print $#a;




__END__
for (@a) {
	print;
}



__END__
$\=undef;
for $t (@space){
	for $p (@$t) {
		local $\ = ",";
		print $p;
	}
	print "\n";
}
