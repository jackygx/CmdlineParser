# CmdlineParser
[EasyCPP] A help class to parse the input argc/argv

Update: 2018/05/19
Version: 1.0

Here is an example on how to use it:

# Initializes a parser.
CCmdlineParser parser(argc, argv);
# Set keys to the parser.
parser.AddKey('h', "Show this help message");
parser.AddKey('a', "Parameter a");
parser.AddKey('b', "Parameter b");
parser.AddKey('c', "Parameter c");
parser.AddKey('d', "Parameter d");
parser.Parse();

# Check if the key 'h' is defined.
# If so, print the usage and exit with 0.
if (parser.IsKeySet('h')) {
	parser.PrintUsage(0);
}

# Check if the key 'a' is defined.
# If not, print the usage and exit with -1.
if (!parser.IsKeySet('a')) {
	parser.PrintUsage(-1);
}

# Get the 1st argument of the key 'a'
CConstStringPtr pa(parser.GetKeyArg('a', 0);

# Get all the arguments of the key 'b'
CStringListPtr list(parser.GetKeyArgs('b');

# Get all the arguments of the key 'c' from lambda
parser.GetKeyArgs('c', [&](const CConstStringPtr &arg) {
	...
});

