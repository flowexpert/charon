<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"
		doctype-public="-//W3C//DTD HTML 4.01//EN"
		doctype-system="http://www.w3.org/TR/html4/strict.dtd"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="basedir" select="'.'"/>
	<xsl:param name="stylesheet" select="'styles.css'"/>
	<xsl:param name="exclude"/>

	<xsl:template match="/">
		<html>
			<head>
				<title>Group Index</title>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href"><xsl:value-of select="$stylesheet"/></xsl:attribute>
				</link>
			</head>
			<body>
				<h1>Module Groups</h1>
				<table class="index">
					<xsl:apply-templates select="doxygenindex/compound[@kind='group']"/>
				</table>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="compound[@kind='group']">
		<xsl:if test="not(name='charon-modules') and not(name=$exclude)">
			<xsl:apply-templates select="document(concat($basedir,'/',@refid,'.xml'))/doxygen/*"/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="compounddef[@kind='group']">
		<tr>
			<td><h2><xsl:value-of select="title"/></h2></td>
			<td><xsl:apply-templates select="briefdescription"/></td>
		</tr>
		<tr><td colspan="2"><xsl:apply-templates select="detaileddescription"/></td></tr>
		<xsl:apply-templates select="innerclass"/>
	</xsl:template>

	<xsl:template match="innerclass">
		<xsl:apply-templates select="document(concat($basedir,'/',@refid,'.xml'))/doxygen/*"/>
	</xsl:template>

	<xsl:template match="compounddef[@kind='class']">
		<tr>
			<td class="firstrow dtype">
				<a class="baseclass">
					<xsl:attribute name="href"><xsl:value-of select="@id" />.html</xsl:attribute>
					<xsl:value-of select="compoundname"/>
				</a>
			</td>
			<td class="firstrow">
				<xsl:apply-templates select="briefdescription"/>
			</td>
		</tr>
	</xsl:template>
</xsl:stylesheet>
