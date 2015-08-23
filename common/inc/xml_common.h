#ifndef __XML_COMMON_H__
#define __XML_COMMON_H__

#define XML_GET_VAULE_TEXT(object, node_name, variable) \
	if (0 == object.name().toString().compare(node_name)) { \
		variable=object.readElementText(); \
		continue; \
	}

#define XML_GET_VAULE_INT(object, node_name, variable) \
	if (0 == object.name().toString().compare(node_name)) { \
		variable=object.readElementText().toInt(); \
		continue; \
	}

#define XML_GET_VALUE_TEXT_BEGIN(object, node_name) \
	if (0 == object.name().toString().compare(node_name)) { \
		QString szBuf=object.readElementText(); 

#define XML_GET_VALUE_TEXT_END }
	
#define XML_GET_ATTR_TEXT(object, node_name, attr_name, variable) \
	if (0 == object.name().toString().compare(node_name)) { \
		QXmlStreamAttributes attr=object.attributes(); \
		variable=attr.value(attr_name).toString(); \
	}

#define XML_GET_ATTR_INT(object, node_name, attr_name, variable) \
	if (0 == object.name().toString().compare(node_name)) { \
		QXmlStreamAttributes attr=object.attributes(); \
		variable=attr.value(attr_name).toString().toInt(); \
	}

#define XML_SET_VAULE(object, node_name, value) \
	object.writeStartElement(node_name); \
	object.writeCharacters(value); \
	object.writeEndElement();

#define XML_SET_ATTR_BEGIN(object, node_name) \
	object.writeStartElement(node_name);

#define XML_SET_ATTR_END(object) \
	object.writeEndElement();

#define XML_SET_ATTR_VALUE(object, attr_name, value) \
	object.writeAttribute(attr_name, value);

#define XML_SET_ATTR_NODE_VALUE(object, value) \
	object.writeCharacters(attr_name, value);


#endif // __XML_COMMON_H__

