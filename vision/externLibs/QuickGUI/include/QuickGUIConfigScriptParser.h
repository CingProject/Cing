//This code is public domain - you can do whatever you want with it
//Original author: John Judnich
#pragma once
#ifndef __QuickGuiSkinSetParser_h_
#define __QuickGuiSkinSetParser_h_

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIExportDLL.h"

#include "OgreScriptLoader.h"
#include "OgreStringVector.h"

namespace QuickGUI
{
	class ConfigNode;

	// necessary to make use of QuickGUI's script parser.
	void _QuickGUIExport registerScriptParser();

	class _QuickGUIExport ConfigScriptLoader: public Ogre::ScriptLoader
	{
	public:
		static ConfigScriptLoader& getSingleton() 
		{
			static ConfigScriptLoader theLog;
			return theLog;
		}

		inline static ConfigScriptLoader *getSingletonPtr() { return &getSingleton(); }

		float getLoadingOrder() const;
		const Ogre::StringVector &getScriptPatterns() const;

		ConfigNode *getConfigScript(const std::string &type, const std::string &name);

		void parseScript(Ogre::DataStreamPtr &stream, const std::string &groupName);

	private:
		ConfigScriptLoader();   // ctor is hidden
		ConfigScriptLoader(ConfigScriptLoader const&);	// copy ctor is hidden
		ConfigScriptLoader& operator=(ConfigScriptLoader const&);	// assign op is hidden
		~ConfigScriptLoader();	// dtor is hidden

		float mLoadOrder;
		Ogre::StringVector mScriptPatterns;

		HashMap<std::string, ConfigNode*> scriptList;

		//Parsing
		char *parseBuff, *parseBuffEnd, *buffPtr;
		size_t parseBuffLen;

		enum Token
		{
			TOKEN_TEXT,
			TOKEN_NEWLINE,
			TOKEN_OPENBRACE,
			TOKEN_CLOSEBRACE,
			TOKEN_EOF,
		};

		Token tok, lastTok;
		std::string tokVal, lastTokVal;
		char *lastTokPos;

		void _parseNodes(ConfigNode *parent);
		void _nextToken();
		void _prevToken();
	};


	class _QuickGUIExport ConfigNode
	{
	public:
		ConfigNode(ConfigNode *parent, const std::string &name = "untitled");
		~ConfigNode();

		inline void setName(const std::string &name)
		{
			this->name = name;
		}

		inline std::string &getName()
		{
			return name;
		}

		inline void addValue(const std::string &value)
		{
			values.push_back(value);
		}

		inline void clearValues()
		{
			values.clear();
		}

		inline Ogre::StringVector &getValues()
		{
			return values;
		}

		ConfigNode *addChild(const std::string &name = "untitled");
		ConfigNode *findChild(const std::string &name, bool recursive = false);

		inline std::vector<ConfigNode*> &getChildren()
		{
			return children;
		}

		void setParent(ConfigNode *newParent);

		inline ConfigNode *getParent()
		{
			return parent;
		}

	private:
		std::string name;
		Ogre::StringVector values;
		std::vector<ConfigNode*> children;
		ConfigNode *parent;

		bool _removeSelf;
	};


	class _QuickGUIExport ConfigScriptSerializer
	{
	public:
		void beginSection(const unsigned short level);
		void endSection(const unsigned short level);
		void writeAttribute(const unsigned short level, const std::string& att);
		void writeValue(const std::string& val);
		void clearQueue();
		void exportQueued(const std::string &fileName);

	private:
		std::string mBuffer;
	};
}

#endif //__QuickGuiSkinSetParser_h_
