import sys
from ambuild2 import run

SOURCE_PATH = '/src'

builder = run.PrepareBuild(sourcePath = sys.path[0] + SOURCE_PATH)

# Builder options
builder.options.add_option('--mms-path', type=str, dest='mms_path', default=None,
                       help='Path to Metamod:Source')
builder.options.add_option('--sm-path', type=str, dest='sm_path', default=None,
                       help='Path to SourceMod')
builder.options.add_option('--enable-debug', action='store_const', const='1', dest='debug',
                       help='Enable debugging symbols')
builder.options.add_option('--enable-optimize', action='store_const', const='1', dest='opt',
                       help='Enable optimization')

builder.Configure()