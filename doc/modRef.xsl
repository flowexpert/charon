<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"
		doctype-public="-//W3C//DTD HTML 4.01//EN"
		doctype-system="http://www.w3.org/TR/html4/strict.dtd"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="stylesheet" select="'styles.css'"/>
	
	<xsl:template match="/">
		<html>
			<head>
				<title><xsl:value-of select="/doxygen/compounddef/compoundname"/></title>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href"><xsl:value-of select="$stylesheet"/></xsl:attribute>
				</link>
			</head>
			<body>
				<xsl:apply-templates select="doxygen/compounddef"/>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="compounddef">
		<h1><xsl:value-of select="compoundname"/> Module Reference</h1>
		<p><xsl:apply-templates select="briefdescription"/></p>
		<p><xsl:apply-templates select="detaileddescription"/></p>
		<table>
			<tr><td colspan="2"><h2>input slots</h2></td></tr>
			<xsl:for-each select="sectiondef/memberdef">
				<xsl:if test="type//ref='InputSlot'">
					<xsl:call-template name="memberdoc"/>
				</xsl:if>
			</xsl:for-each>
			<tr><td colspan="2"><h2>output slots</h2></td></tr>
			<xsl:for-each select="sectiondef/memberdef">
				<xsl:if test="type//ref='OutputSlot'">
					<xsl:call-template name="memberdoc"/>
				</xsl:if>
			</xsl:for-each>
			<tr><td colspan="2"><h2>parameters</h2></td></tr>
			<xsl:for-each select="sectiondef/memberdef">
				<xsl:if test="starts-with(type//ref, 'Parameter')">
					<xsl:call-template name="memberdoc"/>
				</xsl:if>
			</xsl:for-each>
		</table>
		<h2>further information</h2>
		<p>
			This Module subclasses <a class="baseclass">
			<!-- need to look for the baseclass in the inheritancegraph to find link -->
			<xsl:for-each select="inheritancegraph/node">
				<xsl:if test="link/@refid=../../basecompoundref/@refid">
					<xsl:attribute name="href">
						<xsl:call-template name="resolveLink">
							<xsl:with-param name="external" select="link/@external"/>
							<xsl:with-param name="refid" select="../../basecompoundref/@refid"/>
						</xsl:call-template>
					</xsl:attribute>
				</xsl:if>
			</xsl:for-each>
			<xsl:value-of select="basecompoundref" /></a>.<br/>
			For documentation of parameters and slots inherited by this base class,
			please have a look at the corresponding Module reference.
		</p>
		<xsl:if test="count(templateparamlist/param) &gt; 0">
			<p>
				This module is templated. There are instances with T=int,float,double.<br/>
				An additional parameter called <span style="font-weight:bold">templatetype</span>
				may be used to select which instantiation you want.
			</p>
		</xsl:if>
		<p>The detailed doxygen documentation beyond the parameters/slots may be found
			<a><xsl:attribute name="href">../html/<xsl:value-of select="@id"/>.html</xsl:attribute>here</a>.
		</p>
	</xsl:template>

	<xsl:template name="memberdoc">
		<tr>
			<td class="dtype firstrow"><xsl:apply-templates select="type"/></td>
			<td class="firstrow"><span class="parname"><xsl:value-of select="name"/></span></td>
		</tr>
		<tr><td>&#160;</td><td><xsl:apply-templates select="briefdescription"/></td></tr>
	</xsl:template>

	<xsl:template name="stripNS">
		<xsl:choose>
			<xsl:when test="contains(.,'::')">
				<xsl:value-of select="substring-after(.,'::')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="."/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="briefdescription">
		<xsl:choose>
			<xsl:when test="string-length() &gt; 9">
				<xsl:apply-templates />
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="../../header"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ref">
		<a class="ref">
			<xsl:attribute name="href">
				<xsl:call-template name="resolveLink"/>
			</xsl:attribute>
			<xsl:call-template name="stripNS"/>
		</a>
	</xsl:template>

	<xsl:template match="formula">
		<img class="formulaInl">
			<xsl:attribute name="alt"><xsl:value-of select="."/></xsl:attribute>
			<xsl:attribute name="src">../html/form_<xsl:value-of select="@id"/>.png</xsl:attribute>
		</img>
	</xsl:template>

	<xsl:template name="resolveLink">
		<xsl:param name="external" select="@external"/>
		<xsl:param name="refid" select="@refid"/>
		<xsl:choose>
			<xsl:when test="$external='CImg.tag'">http://cimg.sourceforge.net/reference/<xsl:value-of select="@refid" />.html</xsl:when>
			<xsl:when test="contains($external,'.tag')">http://charon-suite.sourceforge.net/doc/<xsl:value-of select="substring-before($external,'.tag')"/>/html/<xsl:value-of select="$refid" />.html</xsl:when>
			<xsl:otherwise><xsl:value-of select="$refid" />.html</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
