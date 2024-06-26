import {expect,test,beforeAll,afterAll} from "bun:test"

beforeAll(async()=>{
    const proc =  Bun.spawn(["make"])
    await proc.exited
})

async function runScript(){
    const proc = Bun.spawn(["./jp"])
    const output = (await new Response(proc.stdout).text())

    await proc.exited

    return output
}

test("Check for Valid Empty JSON",async()=>{
    const output = await runScript()

    expect(output).toEqual("Succesfully Parsed\n")
})

afterAll(()=>{
    const proc = Bun.spawn(['make','clean'])
    proc.exited
})

