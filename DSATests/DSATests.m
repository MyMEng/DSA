//
//  DSATests.m
//  DSATests
//
//  Created by Jakub Zalewski on 30/01/2013.
//  Copyright (c) 2013 Jakub Zalewski. All rights reserved.
//

#import "DSATests.h"
#import "matrix.h"
#import "string.h"

@implementation DSATests

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testExample
{

    stage1("input/set1-stage1.input", 3, 3);
}

- (void)testSimpleParsing
{
    // Simple parsing tests
    char sample[] = "2,3";
    char *entry = sample;
    int dims[] = {0, 0, 0, 0};
    int result = parseEntry(&entry, dims);
    STAssertTrue(dims[0] == 2 && dims[1] == 3, @"Dimension wrong");
    STAssertEquals(result, 2, @"Parsed numbers mismatch");
    STAssertTrue(entry == NULL, @"Wrong reminder");
}

-(void)testAnotherParsing
{
    char identity[] =  "3,3\n"
                        "0,0,1\n"
                        "1,1,1\n"
                        "2,2,1\n";
    char *entry = identity;
    
    int dims[] = {0, 0, 0, 0};
    int result = parseEntry(&entry, dims);
    STAssertEquals(result, 2, @"Parsed number mismatch");
    STAssertTrue(dims[0] == 3 && dims[1] == 3 && dims[2] == 0, @"Int value mismatch");
    // Testing the remainder
    char rest1[] =  "0,0,1\n"
                    "1,1,1\n"
                    "2,2,1\n";
    STAssertEquals(strcmp(entry, rest1), 0, @"Wrong reminder");
    result = parseEntry(&entry, dims);
    STAssertEquals(result, 3, @"Parsed number mismatch");
    STAssertTrue(dims[0] == 0 && dims[1] == 0 && dims[2] == 1, @"Int value mismatch");
    // Testing the remainder
    char rest2[] =  "1,1,1\n"
                    "2,2,1\n";
    STAssertEquals(strcmp(entry, rest2), 0, @"Wrong reminder");
    result = parseEntry(&entry, dims);
    STAssertEquals(result, 3, @"Parsed number mismatch");
    STAssertTrue(dims[0] == 1 && dims[1] == 1 && dims[2] == 1, @"Int value mismatch %d %d %d", dims[0], dims[1], dims[2]);
    // Testing the remainder
    char rest3[] = "2,2,1\n";
    STAssertEquals(strcmp(entry, rest3), 0, @"Wrong reminder %s", entry);
    result = parseEntry(&entry, dims);
    STAssertEquals(result, 3, @"Parsed number mismatch");
    STAssertTrue(dims[0] == 2 && dims[1] == 2 && dims[2] == 1, @"Int value mismatch");
    // Testing the remainder
    char rest4[] =  "";
    STAssertEquals(strcmp(entry, rest4), 0, @"Wrong reminder");
}

-(void)testLineCounting
{
    char test1[] = "\n";
    char test2[] = "\n\n";
    char test3[] =  "sample random\n"
                    "test without last line";
    char test4[] =  "sample random text\n"
                    "with additional last line\n"
                    "\n";
    char test5[] = "\n\n\n\nSome\n\nS\n\nL\n\n";
    
    STAssertEquals(countLines(test1), 0, @"wrong count of lines");
    STAssertEquals(countLines(test2), 0, @"wrong count of lines");
    STAssertEquals(countLines(test3), 2, @"wrong count of lines");
    STAssertEquals(countLines(test4), 2, @"wrong count of lines");
    STAssertEquals(countLines(test5), 3, @"wrong count of lines");
}


@end
