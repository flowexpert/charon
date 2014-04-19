<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" encoding="UTF-8" indent="no"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="kind" select="'class'"/>

	<xsl:template match="/">
		<xsl:choose>
			<xsl:when test="$kind='class' or $kind='group'">
				<xsl:apply-templates
					select="doxygenindex/compound[@kind=$kind]"
					mode="standard"/>
			</xsl:when>
			<xsl:when test="$kind='index'">
				<xsl:apply-templates
					select="doxygenindex/compound[@kind='group']"
					mode="index"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:message terminate="yes">
					<xsl:text>given index kind "</xsl:text>
					<xsl:value-of select="$kind"/>
					<xsl:text>" not recognized</xsl:text>
				</xsl:message>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="compound" mode="standard">
		<xsl:if test="@kind='group' or not(starts-with(@refid,'classstd'))">
			<xsl:value-of select="@refid"/>
			<xsl:text>&#10;</xsl:text>
		</xsl:if>
	</xsl:template>

	<xsl:template match="compound" mode="index">
		<xsl:if test="name='charon-modules'">
			<xsl:value-of select="@refid"/>
			<xsl:text>&#10;</xsl:text>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
