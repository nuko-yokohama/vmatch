/* contrib/vmatch/vmatch--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION vmatch" to load this file. \quit

CREATE FUNCTION get_similar_rate(text, text)
RETURNS float4
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION vmatch(text, text)
RETURNS boolean
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE OPERATOR /= (
    leftarg = text,
    rightarg = text,
    procedure = vmatch,
    commutator = /=,
    RESTRICT = eqsel
);

