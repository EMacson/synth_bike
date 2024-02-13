SerialPort.listDevices;
~port = SerialPort.new("COM5", 115200);
s.boot;
s.meter;

// interpret result from arduino
(
~charArray = [ ];
~getValues = Routine.new({
	var ascii;
	{
		ascii = ~port.read.asAscii;
		if(ascii.isDecDigit, {~charArray = ~charArray.add(ascii)});
		if(ascii == $a, {
			//convert
			~val = ~charArray.collect(_.digit).convertDigits;
			//empty
			~charArray = [ ];
		});
	}.loop;
}).play;
)

// synth defintition
// taken from https://youtu.be/_NpivsEva5o?si=p5Xt3qFR_BzU9GIP
(
SynthDef.new(\saw, {
	arg cutoff=1000;
	var sig;
	sig = Saw.ar([50,51]);
	sig = RLPF.ar(sig, cutoff.lag(0.02), 0.25, 0.2);
	Out.ar(0,sig);
}).add;
)

~synth = Synth(\saw, [\cutoff, ~val]);
(
~control = Routine.new({
	{
		~synth.set(\cutoff, ~val);
		0.001.wait;
	}.loop;
}).play;
)

~control.stop;
~synth.free;
~getValues.stop;
~port.close;