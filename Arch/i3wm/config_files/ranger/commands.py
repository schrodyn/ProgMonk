
from ranger.api.commands import *


class move_in_dir(Command):

	def execute(self):
		import os.path
		destination = os.path.realpath(self.arg(1))

		if os.path.isdir(destination):
			self.fm.move(right=1)

