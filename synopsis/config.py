from Synopsis.Config import Base
class Config (Base):
    class Linker (Base.Linker):
	class Cxx (Base.Linker.Linker):
	    comment_processors = ['java']
	modules = {
	    'C++':Cxx,
	}
