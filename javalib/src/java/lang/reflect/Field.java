/*
 * @(#)Field.java	1.42 04/05/11
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
package java.lang.reflect;

import org.mini.reflect.ReflectField;

/**
 * A <code>Field</code> provides information about, and dynamic access to, a
 * single field of a class or an interface. The reflected field may be a class
 * (static) field or an instance field.
 *
 * <p>
 * A <code>Field</code> permits widening conversions to occur during a get or
 * set access operation, but throws an <code>IllegalArgumentException</code> if
 * a narrowing conversion would occur.
 *
 * @see Member
 * @see java.lang.Class
 * @see java.lang.Class#getFields()
 * @see java.lang.Class#getField(String)
 * @see java.lang.Class#getDeclaredFields()
 * @see java.lang.Class#getDeclaredField(String)
 *
 * @author Kenneth Russell
 * @author Nakul Saraiya
 */
public final class Field {

    Class clazz;
    ReflectField refField;

    public Field(Class cl, ReflectField reff) {
        refField = reff;
        clazz = cl;
    }

    public String getName() {
        return refField.fieldName;
    }
    
    
    @Override
    public boolean equals(Object o){
        return refField==o;
    }
}
