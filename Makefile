#
#	Top level makefile for Maude system.
#
ROOT_DIR := .

include $(ROOT_DIR)/MakeOptions

whole:
	cd $(ROOT_DIR)/Core; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Interface; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Variable; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Higher; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/FreeTheory; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/ACU_Theory; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/ACU_Persistent; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/CUI_Theory; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/AU_Theory; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/AU_Persistent; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/S_Theory; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/NA_Theory; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Utility; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Temporal; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/BuiltIn; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/MSCP10; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Meta; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/ObjectSystem; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/System; $(MAKE) clean; $(MAKE) -e
	cd $(ROOT_DIR)/Mixfix; $(MAKE) clean; $(MAKE) -e

#	cd $(ROOT_DIR)/FullCompiler; $(MAKE) clean; $(MAKE) -e


clean:
	cd $(ROOT_DIR)/Core; $(MAKE) clean
	cd $(ROOT_DIR)/Interface; $(MAKE) clean
	cd $(ROOT_DIR)/Variable; $(MAKE) clean
	cd $(ROOT_DIR)/Higher; $(MAKE) clean
	cd $(ROOT_DIR)/FreeTheory; $(MAKE) clean
	cd $(ROOT_DIR)/ACU_Theory; $(MAKE) clean
	cd $(ROOT_DIR)/ACU_Persistent; $(MAKE) clean
	cd $(ROOT_DIR)/CUI_Theory; $(MAKE) clean
	cd $(ROOT_DIR)/AU_Theory; $(MAKE) clean
	cd $(ROOT_DIR)/AU_Persistent; $(MAKE) clean
	cd $(ROOT_DIR)/S_Theory; $(MAKE) clean
	cd $(ROOT_DIR)/NA_Theory; $(MAKE) clean
	cd $(ROOT_DIR)/Utility; $(MAKE) clean
	cd $(ROOT_DIR)/Temporal; $(MAKE) clean
	cd $(ROOT_DIR)/BuiltIn; $(MAKE) clean
	cd $(ROOT_DIR)/MSCP10; $(MAKE) clean
	cd $(ROOT_DIR)/FullCompiler; $(MAKE) clean
	cd $(ROOT_DIR)/Meta; $(MAKE) clean
	cd $(ROOT_DIR)/ObjectSystem; $(MAKE) clean
	cd $(ROOT_DIR)/System; $(MAKE) clean
	cd $(ROOT_DIR)/Mixfix; $(MAKE) clean
